# videocore-kernels
Compute kernels utilizing the vector processor on the Raspberry Pi

### thresholdCentroid
Computes the centroid of pixels in an image which exceed a given threshold. Works on 8-bit grayscale images of any size up to 1920 pixels wide. The image size and threshold can be specified at runtime. This kernel demonstrates vertical and horizontal accesses of 8, 16, and 32 bit widths, an interleaved load/store operation, use of the accumulators, and the scalar register update.

### VideoCoreIV Instruction Set
See https://github.com/hermanhermitage/videocoreiv/wiki/VideoCore-IV-Programmers-Manual

### vasm
Use [vasm](http://sun.hasenbraten.de/vasm/index.php?view=relsrc) to assemble videocore kernels into binary. Vasm requires the included patch to correctly assemble the thresholdCentroid kernel. The patch corrects one bug involving the 80-bit variant of vld/vst instructions, and a second bug invloving the vcmpge instruction.

### Requirements
You need [librpimemmgr](https://github.com/Idein/librpimemmgr) and
[mailbox](https://github.com/Terminus-IMRC/mailbox).
