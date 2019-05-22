﻿#pragma once

#include "stdafx.h"
#include "NeuQuantizer.h"
#include "bitmapUtilities.h"
#include "CIELABConvertor.h"
#include <unordered_map>

namespace NeuralNet
{
	//====================
	// NeuralNet Color quantizing

	/* NeuQuant Neural-Net Quantization Algorithm
	* ------------------------------------------
	*
	* Copyright (c) 1994 Anthony Dekker
	* Copyright (c) 2019 Miller Cy Chan
	*
	* NEUQUANT Neural-Net quantization algorithm by Anthony Dekker, 1994.
	* See "Kohonen neural networks for optimal colour quantization"
	* in "Network: Computation in Neural Systems" Vol. 5 (1994) pp 351-367.
	* for research paper of the algorithm.
	* See also  https://www.researchgate.net/publication/232079905_Kohonen_neural_networks_for_optimal_colour_quantization
	*
	* Any party obtaining a copy of these files from the author, directly or
	* indirectly, is granted, free of charge, a full and unrestricted irrevocable,
	* world-wide, paid up, royalty-free, nonexclusive right and license to deal
	* in this software and documentation files (the "Software"), including without
	* limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
	* and/or sell copies of the Software, and to permit persons who receive
	* copies from any such party to do so, with the only requirement being
	* that this copyright notice remain intact.
	*/

	double PR = .2126, PG = .7152, PB = .0722;
	const short specials = 3;		// number of reserved colours used
	const int ncycles = 115;			// no. of learning cycles
	const int radiusbiasshift = 8;
	const int radiusbias = 1 << radiusbiasshift;

	int netsize = 256;		// number of colours used	
	int maxnetpos = netsize - 1;
	int initrad = netsize >> 3;   // for 256 cols, radius starts at 32
	double initradius = initrad * 1.0;
	const int radiusdec = 30; // factor of 1/30 each cycle

	const int alphabiasshift = 10;			// alpha starts at 1
	const int initalpha = 1 << alphabiasshift; // biased by 10 bits
	const int alpharadbshift = alphabiasshift + radiusbiasshift;
	const double alpharadbias = (double)(1 << alpharadbshift);

	/* defs for freq and bias */
	const int gammashift = 10;                  /* gamma = 1024 */
	const double gamma = (double)(1 << gammashift);
	const int betashift = 10;
	const double beta = (1.0 / (double)(1 << betashift));/* beta = 1/1024 */
	const double betagamma = (double)(1 << (gammashift - betashift));

	struct nq_pixel
	{
		double al, L, A, B;
	};

	unique_ptr<nq_pixel[]> network; // the network itself

	unique_ptr<unsigned short[]> netindex; // for network lookup - really 256

	unique_ptr<double[]> bias;  // bias and freq arrays for learning
	unique_ptr<double[]> freq;
	unique_ptr<double[]> radpower;

	// four primes near 500 - assume no image has a length so large
	// that it is divisible by all four primes

	const int primes[] = { 499, 491, 487, 503 };
	double gamma_correction = 1.0;         // 1.0/2.2 usually

	bool hasSemiTransparency = false;
	int m_transparentPixelIndex = -1;
	ARGB m_transparentColor = Color::Transparent;
	unordered_map<ARGB, CIELABConvertor::Lab> pixelMap;

	inline double colorimportance(double al)
	{
		double transparency = 1.0 - al / 255.0;
		return (1.0 - transparency * transparency);
	}

	void SetUpArrays() {
		network = make_unique<nq_pixel[]>(netsize);
		netindex = make_unique<unsigned short[]>(max(netsize, 256));
		bias = make_unique<double[]>(netsize);
		freq = make_unique<double[]>(netsize);
		radpower = make_unique<double[]>(initrad);

		for (int i = specials; i < netsize; ++i) {
			network[i].L = network[i].A = network[i].B = i / netsize;

			/*  Sets alpha values at 0 for dark pixels. */
			if (i < 16)
				network[i].al = i * 16.0;
			else
				network[i].al = BYTE_MAX;

			freq[i] = 1.0 / netsize;
		}
	}

	void getLab(const Color& c, CIELABConvertor::Lab& lab1)
	{
		auto got = pixelMap.find(c.GetValue());
		if (got == pixelMap.end()) {
			CIELABConvertor::RGB2LAB(c, lab1);
			pixelMap[c.GetValue()] = lab1;
		}
		else
			lab1 = got->second;
	}

	inline UINT round_biased(double temp)
	{
		if (temp < 0)
			return 0;
		temp = floor((temp / 255.0 * 256.0));

		if (temp > BYTE_MAX)
			return BYTE_MAX;
		return (UINT)temp;
	}

	void Altersingle(double alpha, UINT i, byte al, double L, double A, double B) {
		double colorimp = 1.0;//0.5;// + 0.7 * colorimportance(al);

		alpha /= initalpha;

		/* alter hit neuron */
		network[i].al -= alpha * (network[i].al - al);
		network[i].L -= colorimp * alpha * (network[i].L - L);
		network[i].A -= colorimp * alpha * (network[i].A - A);
		network[i].B -= colorimp * alpha * (network[i].B - B);
	}

	void Alterneigh(UINT rad, UINT i, byte al, double L, double A, double B) {
		int lo = i - rad;
		if (lo < 0)
			lo = 0;
		UINT hi = i + rad;
		if (hi > maxnetpos)
			hi = maxnetpos;

		UINT j = i + 1;
		int k = i - 1;
		auto q = radpower.get();
		while ((j <= hi) || (k >= lo)) {
			double a = (*(++q)) / alpharadbias;
			if (j <= hi) {
				network[j].al -= a * (network[j].al - al);
				network[j].L -= a * (network[j].L - L);
				network[j].A -= a * (network[j].A - A);
				network[j].B -= a * (network[j].B - B);
				j++;
			}
			if (k >= lo) {
				network[k].al -= a * (network[k].al - al);
				network[k].L -= a * (network[k].L - L);
				network[k].A -= a * (network[k].A - A);
				network[k].B -= a * (network[k].B - B);
				k--;
			}
		}
	}

	UINT Contest(byte al, double L, double A, double B) {
		/* finds closest neuron (min dist) and updates freq */
		/* finds best neuron (min dist-bias) and returns position */
		/* for frequently chosen neurons, freq[i] is high and bias[i] is negative */
		/* bias[i] = gamma*((1/netsize)-freq[i]) */

		UINT bestpos = 0, bestbiaspos = bestpos;
		double bestd = INT_MAX, bestbiasd = bestd;

		/* Using colorimportance(al) here was causing problems with images that were close to monocolor.
		See bug reports: 3149791, 2938728, 2896731 and 2938710
		*/
		double colimp = 1.0; //colorimportance(al); 

		for (int i = 0; i < netsize; ++i) {
			double bestbiasd_biased = bestbiasd + bias[i];

			double a = network[i].L - L;
			double dist = abs(a) * colimp;
			a = network[i].A - A;
			dist += abs(a) * colimp;

			if (dist < bestd || dist < bestbiasd_biased) {
				a = network[i].B - B;
				dist += abs(a) * colimp;
				a = network[i].al - al;
				dist += abs(a);

				if (dist < bestd) {
					bestd = dist;
					bestpos = i;
				}
				if (dist < bestbiasd_biased) {
					bestbiasd = dist - bias[i];
					bestbiaspos = i;
				}
			}

			double betafreq = freq[i] * beta;
			freq[i] -= betafreq;
			bias[i] += betafreq * gamma;
		}
		freq[bestpos] += beta;
		bias[bestpos] -= betagamma;
		return bestbiaspos;
	}

	void Learn(const int samplefac, const vector<ARGB>& pixels) {
		UINT stepIndex = 0;

		int pos = 0;
		int alphadec = 30 + ((samplefac - 1) / 3);
		const UINT lengthcount = pixels.size();
		UINT samplepixels = lengthcount / samplefac;
		UINT delta = samplepixels / ncycles;  /* here's a problem with small images: samplepixels < ncycles => delta = 0 */
		if (delta == 0)
			delta = 1;        /* kludge to fix */
		double alpha = initalpha;
		double radius = initradius;

		UINT rad = (UINT)radius;
		if (rad <= 1)
			rad = 0;

		for (UINT i = 0; i < rad; ++i)
			radpower[i] = floor(alpha * (((sqr(rad) - sqr(i)) * radiusbias) / sqr(rad)));

		UINT i = 0;
		while (i < samplepixels) {
			Color c(pixels[pos]);

			byte al = c.GetA();
			CIELABConvertor::Lab lab1;
			getLab(c, lab1);

			auto j = Contest(al, lab1.L, lab1.A, lab1.B);

			Altersingle(alpha, j, al, lab1.L, lab1.A, lab1.B);
			if (rad)
				Alterneigh(rad, j, al, lab1.L, lab1.A, lab1.B);   /* alter neighbours */

			pos += primes[stepIndex++ % 4];
			while (pos >= lengthcount)
				pos -= lengthcount;

			if (++i % delta == 0) {                    /* FPE here if delta=0*/
				alpha -= alpha / (double)alphadec;
				radius -= radius / (double)radiusdec;
				rad = (UINT)radius;
				if (rad <= 1)
					rad = 0;
				for (UINT j = 0; j < rad; ++j)
					radpower[j] = floor(alpha * (((sqr(rad) - sqr(j)) * radiusbias) / sqr(rad)));
			}
		}
	}

	void Inxbuild(ColorPalette* pPalette) {
		UINT nMaxColors = pPalette->Count;		

		int previouscol = 0;
		int startpos = 0;

		for (int i = 0; i < nMaxColors; ++i) {
			Color c(pPalette->Entries[i]);
			int smallpos = i;
			auto smallval = network[i].L;			// index on L
											// find smallest in i..netsize-1
			for (int j = i + 1; j < nMaxColors; ++j) {
				if (network[j].L < smallval) {		// index on L				
					smallpos = j;
					smallval = network[j].L;	// index on L
				}
			}
			// swap p (i) and q (smallpos) entries
			if (i != smallpos)
				swap(network[smallpos], network[i]);

			// smallval entry is now in position i
			if (smallval != previouscol) {
				netindex[previouscol] = (startpos + i) >> 1;
				for (int j = previouscol + 1; j < smallval; ++j)
					netindex[j] = i;
				previouscol = smallval;
				startpos = i;
			}
		}

		netindex[previouscol] = (startpos + maxnetpos) >> 1;
		for (int j = previouscol + 1; j < netsize; ++j)
			netindex[j] = maxnetpos;

		for (UINT k = 0; k < nMaxColors; ++k) {
			CIELABConvertor::Lab lab1;
			lab1.alpha = round_biased(network[k].al);
			lab1.L = network[k].L, lab1.A = network[k].A, lab1.B = network[k].B;
			pPalette->Entries[k] = CIELABConvertor::LAB2RGB(lab1);
		}
	}

	short nearestColorIndex(const ColorPalette* pPalette, const UINT nMaxColors, const ARGB argb)
	{
		short k = 0;
		Color c(argb);

		double mindist = INT_MAX;
		for (short i = 0; i < nMaxColors; i++) {
			Color c2(pPalette->Entries[i]);
			double curdist = sqr(c2.GetA() - c.GetA());
			if (curdist > mindist)
				continue;

			curdist += PR * sqr(c2.GetR() - c.GetR());
			if (curdist > mindist)
				continue;

			curdist += PG * sqr(c2.GetG() - c.GetG());
			if (curdist > mindist)
				continue;

			curdist += PB * sqr(c2.GetB() - c.GetB());
			if (curdist > mindist)
				continue;

			mindist = curdist;
			k = i;
		}
		return k;
	}

	bool quantize_image(const vector<ARGB>& pixels, const ColorPalette* pPalette, const UINT nMaxColors, short* qPixels, const UINT width, const UINT height, const bool dither)
	{
		if (dither)
			return dither_image(pixels.data(), pPalette, nearestColorIndex, hasSemiTransparency, m_transparentPixelIndex, nMaxColors, qPixels, width, height);

		UINT pixelIndex = 0;
		for (UINT j = 0; j < height; ++j) {
			for (UINT i = 0; i < width; ++i)
				qPixels[pixelIndex++] = nearestColorIndex(pPalette, nMaxColors, pixels[pixelIndex]);
		}

		return true;
	}

	void Clear() {
		network.reset();
		netindex.reset();
		bias.reset();
		freq.reset();
		radpower.reset();

		pixelMap.clear();
	}

	// The work horse for NeuralNet color quantizing.
	bool NeuQuantizer::QuantizeImage(Bitmap* pSource, Bitmap* pDest, UINT& nMaxColors, bool dither)
	{
		const UINT bitmapWidth = pSource->GetWidth();
		const UINT bitmapHeight = pSource->GetHeight();

		vector<ARGB> pixels(bitmapWidth * bitmapHeight);
		GrabPixels(pSource, pixels, hasSemiTransparency, m_transparentPixelIndex, m_transparentColor);

		if (nMaxColors > 32768)
			nMaxColors = 32768;

		auto pPaletteBytes = make_unique<byte[]>(sizeof(ColorPalette) + nMaxColors * sizeof(ARGB));
		auto pPalette = (ColorPalette*)pPaletteBytes.get();
		pPalette->Count = nMaxColors;

		netsize = nMaxColors;		// number of colours used
		maxnetpos = netsize - 1;
		initrad = netsize < 8 ? 1 : (netsize >> 3);
		initradius = initrad * 1.0;

		SetUpArrays();
		Learn(dither ? 5 : 1, pixels);
		Inxbuild(pPalette);

		auto qPixels = make_unique<short[]>(pixels.size());
		if (nMaxColors > 256) {
			hasSemiTransparency = false;
			dithering_image(pixels.data(), pPalette, nearestColorIndex, hasSemiTransparency, m_transparentPixelIndex, nMaxColors, qPixels.get(), bitmapWidth, bitmapHeight);
			Clear();
			return ProcessImagePixels(pDest, qPixels.get(), m_transparentPixelIndex);
		}

		if (hasSemiTransparency)
			PR = PG = PB = 1;
		quantize_image(pixels, pPalette, nMaxColors, qPixels.get(), bitmapWidth, bitmapHeight, dither);
		if (m_transparentPixelIndex >= 0) {
			UINT k = qPixels[m_transparentPixelIndex];
			if (nMaxColors > 2)
				pPalette->Entries[k] = m_transparentColor;
			else if (pPalette->Entries[k] != m_transparentColor)
				swap(pPalette->Entries[0], pPalette->Entries[1]);
		}

		Clear();
		return ProcessImagePixels(pDest, pPalette, qPixels.get());
	}
}
