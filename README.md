# nQuantCpp
nQuantCpp includes top 6 color quantization algorithms for visual c++ producing high quality optimized images. I enhance each of the algorithms to support semi transparent images. 
nQuantCpp also provides a command line wrapper in case you want to use it from the command line.

Either download nQuantCpp from this site or add it to your Visual Studio project seamlessly.
PNG is useful because it's the only widely supported format which can store partially transparent images. The format uses compression, but the files can still be large. Use Color quantization algorithms can be chosen by command line since version 1.10 using the /a algorithm.
Only png can support semi transparent image and desired color depth. Gif can ensure the number of colors for the converted image is 256 or less. Bmp does support desired color depth. Jpg only supports 24-bit image format.

Let's climb up the mountain: Ready, Go!!!

<p>Original photo of climbing<br /><img src="https://mcychan.github.io/PnnQuant.js/demo/img/climb.jpg" /></p>
<p>Reduced to 256 colors by Divisive hierarchical clustering algorithm<br /><img src="https://github.com/user-attachments/assets/31a8c472-16ca-47ea-8f55-8143ad761eab" /></p>
<p>Reduced to 256 colors by NeuQuant Neural-Net Quantization Algorithm<br /><img src="https://github.com/user-attachments/assets/26e00c41-730b-4e23-81f7-a7dec4dc4cf6" /></p>
<p>Reduced to 16 colors by Fast pairwise nearest neighbor based algorithm<br /><img src="https://github.com/user-attachments/assets/f4cadcfc-2770-4cb4-b99e-302c9b5db2ae" /></p>
<p>Reduced to 16 colors by Fast pairwise nearest neighbor based algorithm with CIELAB color space<br /><img src="https://github.com/user-attachments/assets/a987c1b5-880d-43f6-8696-aa533aa61be5" /></p>
<p>Reduced to 16 colors by Xialoin Wu's fast optimal color Quantization Algorithm<br /><img src="https://github.com/user-attachments/assets/f0d26906-4b2b-42de-ae98-f660645cc198" /></p>
<hr />
<p>Original photo of Aetna's Hartford headquarters<br /><img src="https://mcychan.github.io/PnnQuant.js/demo/img/SE5x9.jpg" /></p>
<p>Reduced to 256 colors by NeuQuant Neural-Net Quantization Algorithm<br /><img src="https://github.com/user-attachments/assets/b1f4c71b-f33e-40c3-aa20-aa94bf3dec3e" /></p>
<p>Reduced to 256 colors by Fast pairwise nearest neighbor based algorithm<br /><img src="https://repository-images.githubusercontent.com/154477414/29446768-6892-49bc-b949-774c1b3f38de" /></p><hr>

<p>Original image of Hong Kong Cuisines<br /><img src="https://mcychan.github.io/PnnQuant.js/demo/img/old-HK.jpg" /></p>
<b><a href="http://www.cs.joensuu.fi/sipu/pub/Threshold-JEI.pdf">Fast pairwise nearest neighbor based algorithm</a></b> with CIELAB color space in 16 colors<br>
High quality and fast<br />
<img src="https://github.com/user-attachments/assets/d0c44261-b01a-4b6d-af31-bd1b7b0f2df5" alt="Fast pairwise nearest neighbor based algorithm with CIELAB color space with CIELAB color space in 16 colors"></p>
<p>Fast pairwise nearest neighbor based algorithm with CIELAB color space <b>Plus (parallel quantum inspired Genetic Algorithm)</b> in 16 colors<br /><img src="https://github.com/user-attachments/assets/c129778d-436c-4a09-b280-6cf456e079f1" alt="Fast pairwise nearest neighbor based algorithm with CIELAB color space with CIELAB color space Plus (parallel quantum inspired Genetic Algorithm) in 16 colors" /></p>
<p><b><a href="http://cg.cs.tsinghua.edu.cn/people/~huanghz/publications/TIP-2015-CombinedColorQuantization.pdf">Efficient, Edge-Aware, Combined Color Quantization and Dithering </a></b> with CIELAB color space in 16 colors<br />
Higher quality for 32 or less colors but slower<br />
<img src="https://user-images.githubusercontent.com/26831069/147355642-d72a9e2b-a47a-463a-bfb5-98d1a1def84a.png" alt="Efficient, Edge-Aware, Combined Color Quantization and Dithering with CIELAB color space in 16 colors"></p>
<p><b><a href="https://people.eecs.berkeley.edu/~dcoetzee/downloads/scolorq/">Spatial color quantization</a></b> with CIELAB color space in 16 colors<br />
Higher quality for 32 or less colors but the slowest<br />
<img src="https://github.com/user-attachments/assets/b0dbdd2e-4800-42c4-89e3-7f3e03d36d8d" alt="Spatial color quantization with CIELAB color space in 16 colors"></p>
All in all, the top 3 color quantization algorithms for 256 colors are:
<ol>
<li>Fast pairwise nearest neighbor based algorithm</li>
<li>Xialoin Wu's fast optimal color quantizer</li>
<li>NeuQuant Neural-Net Quantization Algorithm with CIELAB color space</li>
</ol>
The top 3 color quantization algorithms for 32 colors or less are:
<ol>
<li>Fast pairwise nearest neighbor based algorithm with CIELAB color space <b>Plus (parallel quantum inspired Genetic Algorithm)</b></li>
<li>Efficient, Edge-Aware, Combined Color Quantization and Dithering algorithm with CIELAB color space</li>
<li>Spatial color quantization algorithm with CIELAB color space</li>
</ol>

If you are using the command line. Assuming you are in the same directory as nQuantCpp.exe, you would enter: `nQuantCpp yourImage.jpg /m 16 /a pnnlab`.<br/>
To avoid dot gain, `/d n` can set the dithering to false. However, false contours will be resulted for gradient color zones.<br />
nQuantCpp will quantize yourImage.jpg with maximum colors 16, algorithm pnnlab and create yourImage-PNNLABquant16.png in the same directory.

The readers can see coding of the error diffusion and dithering are quite similar among the above quantization algorithms. 
Each algorithm has its own advantages. I share the source of color quantization to invite further discussion and improvements.
Such source code are written in C++ to gain best performance. It is readable and convertible to <a href="https://github.com/mcychan/nQuant.cs">c#</a>, <a href="https://github.com/mcychan/nQuant.j2se">java</a>, or <a href="https://github.com/mcychan/PnnQuant.js">javascript</a>.
Welcome for C++ experts for further improvement or provide color quantization algorithms better than the above algorithms.
Please use issues to track ideas, enhancements, tasks, or bugs for work on GitHub.
