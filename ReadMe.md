# Introduction

This project is for practice: _interpolation_.

Using original image 512X512, it will be down-scaled 1/4\. After then interpolation apply for restore original image by up-scaling 4 per each methods. Those methods are as like this:

## Bilinear interpolation

- It is composed linear interpolation in both x and y directions. Because of commutative law, whatever direction you take first results will stay same. Linear interpolation process based on distance and give to weighted value.

## Lagrange interpolation

- According to N-th polynomial of degree, make Lagrange function passing through N+1 data points. Each points have same function value with original function. In order to make this possible, except one point other points should be zero in each points.

## Six-tab interpolation

- Using 6 Tab FIR filter[coefficient: (-1,-5,20,20,-5,1)/32] make half-pixel with reference to the sides 6 of pixel in y direction which is located in the middle of the original pixels. And repeat this manner in the horizontal direction to make half-pixel. Then using 2 Tab Average filter[coefficient: (1/2, 1/2)] make quarter-pixel with two adjacent pixels per vertical, horizontal, and diagonal directions.

## Design detail

> Given all method commonly should fill the remaining three pixels while occurred enlargement. So reduction method is treated to copy from the 512x512 array to 128x128 array depending on multiplied by four pixels from 0 to 127\. This downsized image of pixels goes back to original position when enlargement has processed. But during the Lagrange and Six-tab interpolation, the boundary has a sloppy problem that those methods need to neighboring pixels but boundary has no adjacent one. So it was used to pad, respectively. And to avoid numerical error, the calculation should be proceeded in integer type not in float type.(Except measurement about RMS and PSNR). So I declared the same size of the array for remembering the amount and count required divisions.

--------------------------------------------------------------------------------

- Bilinear interpolation As previously stated, even if either the horizontal axis or the vertical axis of what is processed first, results are same by the commutative law. Because the bilinear system is based on linear. So I firstly start interpolation on the horizontal axis. With respect to new three empty pixels between original two value(interval), I can interpolate that this function. F(x)= ((sf-k))/sf F(x_start )+k/sf F(x_end )<br>
  (k=1,2,...,sf-1 and sf is scaling factor,start and end mean interval) In here sf is four, so this function shall be implemented 3 times about empty pixels. This manner process based on distance and give to weighted value in each point. But we should avoid numerical error caused by dividing so multiplying scaling factor to each interval point. After filling all empty pixels on horizontal axis except boundary (because the boundary has no interval about neighbor pixel) the process proceeds to the vertical axis in the same way only changed axis direction. But in this time scaled factor should divide twice about gained value in vertical step, since we refer to one scaled values. Finally remaining boundary region is padded from the original image. After then write new raw file through the bilinear filter.

--------------------------------------------------------------------------------

- Lagrange interpolation This method needs more neighbor pixels than bilinear so padding range is spread four cardinal points north, south, east, and west. So our enlargement 512X512 image is located in the middle area (Likewise padding is copied nearest boundary value). The third Lagrange equation made by using four pixels over scaled four as follows: F(x)=(x-4)(x-8)(x-12)/(0-4)(0-8)(0-12) F(0)+(x-0)(x-8)(x-12)/(4-0)(4-8)(4-12) F(4)+(x-0)(x-4)(x-12)/(8-0)(8-4)(8-12) F(8)+(x-0)(x-4)(x-8)/(12-0)(12-4)(12-8) F(12) Because of scaled four, the initial values in the expanded image are spaced apart by 4 intervals so this equation includes quadruple numbers. We interpolated with each 4 pixels for one value by applying this Lagrange equation in the horizontal axis and vertical axis. Like a bilinear method, no matter what axis first implemented and second applied values should divide twice with a scaling factor.

--------------------------------------------------------------------------------

- Six-tab interpolation The Six-tab method needs 6 pixels to making one interpolated pixel. To boundary process, padding size is double compared to Lagrange method. Additionally, the diagonal area is needed for making half-pixels both the horizontal axis and vertical axis. But some element has negative coefficient among six-tab filter. So during calculation, results are likely to come out negative. In the raw file, it does not have a negative value about each pixel, which should change zero for only positive value located.

## Result

Method       |      RMS |      PSNR
------------ | -------: | --------:
Bilinear     | 9.440052 | 28.631316
3th-Lagrange | 9.286052 | 28.774181
Six-tap      | 9.538337 | 28.541351

![1](https://cloud.githubusercontent.com/assets/20148930/23831894/43a75756-076d-11e7-8752-ea7b99c44b12.JPG)
