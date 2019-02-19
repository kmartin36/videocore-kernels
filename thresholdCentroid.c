#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rpimemmgr.h>
#include <mailbox.h>

//gcc thresholdCentroid.c -DUSE_VCHIQ_ARM -o thresholdCentroid -rdynamic -lrpimemmgr -lbcm_host -lvcos -lvchiq_arm -pthread -lvcsm -lvcos -lmailbox -lvchiq_arm -lvcsm -lmailbox -I/opt/vc/include -L/opt/vc/lib

unsigned short int code[] = {
  /*
   * thresholdCentroid(unsigned char *image, int width, int height, unsigned char *output, int *sum, int threshold)
  push r6-r9
  vmov HX(0,32), HX(0,32), 0
  mov r6, r0
  vadd VX(0,33++), VX(0,32++), 2 REP 16
  lsr r0, r1, 5
  mov r1, r6
  vadd HX(1++,32), HX(0++,32), 32 REP r0
  eor r7, r7
  addscale16 r8, r7, 2
  veor HY(61, 0), HY(61, 0), HY(61, 0)
  veor HY(62++, 0), HY(62++, 0), HY(62++, 0) REP 2
  eor r9, r9

beginLoop:
    vldh HX(0++,0), (r1+=r8) REP r0
    addscale32 r1, r1, r0
    vcmpge H(0++,0), H(0++,0), r5 REP r0    # assembled incorrectly, X needs to be 0
    vcmpge H(0++,16), H(0++,16), r5 REP r0  # assembled incorrectly, X needs to be 0
    vmov -, H(0++,16), H(0++,16) REP r0 CLRA UACC
    vmov HY(62, 0), HY(62, 0), HY(62, 0) UADD
    vmov -, H(0++,0), H(0++,0) REP r0 UACC
    vsth HX(0++,0), (r3+=r8) REP r0
    addscale32 r3, r3, r0

    vreadacc HY(63, 0), 0
    vadd HY(61, 0), HY(61, 0), HY(63, 0)
    vmov -, HY(63, 0), HY(63, 0) SUMU r6
    mul r6, r9
    add r7, r6

    vmul32u -, H(0++,0), HX(0++,32) REP r0 CLRA UACC
    vmul32u -, H(0++,16), HX(0++,32) REP r0 UACC
    vmov HY(62, 0), HY(62, 0), HY(62, 0) UADD

    addcmpbcs r9, 1, r2, beginLoop

  vmov HY(61, 0), HY(61, 0), HY(61, 0) SUMU r0
  vmov HY(62, 0), HY(62, 0), HY(62, 0) SUMU r1
  st r1, (r4)
  st r7, 4(r4)
  pop r6-r9
  b lr
   */
  
  0x02a3,
  0xf600, 0xa028, 0x0400,
  0x4006,
  0xff04, 0xb06c, 0x0402, 0xfbe0, 0x0000,
  0xc340, 0x0f45,
  0x4061,
  0xff07, 0xa068, 0x0420, 0xfbe0, 0x0000,
  0x4577,
  0xc2e8, 0x3f42,
  0xf690, 0xcf73, 0xd33d,
  0xfe91, 0xcfb3, 0xe33e, 0xfbe0, 0x003e,
  0x4599,
//beginLoop:
  0xf80f, 0x8038, 0x0380, 0xfa00, 0x0004,
  0xc581, 0x0f00,
  0xfcff, 0x0000, 0x0380, 0xfbe0, 0x0014,
  0xfcff, 0x2008, 0x0380, 0xfbe0, 0x0014,
  0xfe07, 0xf008, 0x0080, 0xf3e0, 0x09be,
  0xfe00, 0xcfb3, 0xe33e, 0xf3c0, 0x083c,
  0xfe07, 0xf000, 0x0000, 0xf3e0, 0x08be,
  0xf88f, 0xe020, 0x0380, 0x83e0, 0x000c,
  0xc583, 0x1f00,
  0xf300, 0xcfc0, 0x0400,
  0xf700, 0xcf73, 0xd33f,
  0xfe00, 0xf033, 0xf33f, 0xf3c0, 0x11bc,
  0x4496,
  0x4267,
  0xffbf, 0xf000, 0x0280, 0xf3e0, 0x09be,
  0xffbf, 0xf008, 0x0280, 0xf3e0, 0x08be,
  0xfe00, 0xcfb3, 0xe33e, 0xf3c0, 0x083c,
  0x8219, 0x4bbd,
  0xfe00, 0xcf73, 0xd33d, 0xf3c0, 0x103c,
  0xfe00, 0xcfb3, 0xe33e, 0xf3c0, 0x107c,
  0x0941,
  0x3147,
  0x0223,
  0x005a
};

int main(int argc, char *argv[]) {
  int file_desc = mailbox_open();
  void *code_usr = NULL, *src_usr = NULL, *dst_usr = NULL, *sums_usr = NULL;
  uint32_t code_bus = 0, src_bus = 0, dst_bus = 0, sums_bus = 0;
  struct rpimemmgr mgr;
  int err = rpimemmgr_init(&mgr);
  unsigned int width = atoi(argv[1]);
  unsigned int height = atoi(argv[2]);
  unsigned int threshold = atoi(argv[3]);
  unsigned int size = width * height;
  if (file_desc < 0)
    return 1;
  if (err)
    return err;
  err = rpimemmgr_alloc_vcsm(sizeof(code), 4096, VCSM_CACHE_TYPE_NONE, &code_usr, &code_bus, &mgr);
  if (err)
    return err;
  err = rpimemmgr_alloc_vcsm(size, 4096, VCSM_CACHE_TYPE_NONE, &src_usr, &src_bus, &mgr);
  if (err)
    return err;
  err = rpimemmgr_alloc_vcsm(size, 4096, VCSM_CACHE_TYPE_NONE, &dst_usr, &dst_bus, &mgr);
  if (err)
    return err;
  err = rpimemmgr_alloc_vcsm(3*sizeof(int), 4096, VCSM_CACHE_TYPE_NONE, &sums_usr, &sums_bus, &mgr);
  if (err)
    return err;
  
  memcpy(code_usr, code, sizeof(code));
  
  FILE *in = fopen(argv[4], "rb");
  fread(src_usr, 1, size, in);
  fclose(in);
  
  int r0;
  for (err=0; err<1; err++)
    r0 = mailbox_execute_code(file_desc, code_bus, src_bus, width, height, dst_bus, sums_bus, threshold);

  FILE *out = fopen(argv[5], "wb");
  fwrite(dst_usr, 1, size, out);
  fclose(out);
  
  float count = r0;
  float xsumf = ((int *)sums_usr)[0];
  float ysumf = ((int *)sums_usr)[1];
  printf("centroid %f %f count %d %d %d\n", xsumf/count, ysumf/count, r0, ((int *)sums_usr)[0], ((int *)sums_usr)[1]);
  
  err = rpimemmgr_free_by_usraddr(code_usr, &mgr);
  if (err)
    return err;
  err = rpimemmgr_free_by_usraddr(src_usr, &mgr);
  if (err)
    return err;
  err = rpimemmgr_free_by_usraddr(dst_usr, &mgr);
  if (err)
    return err;
  err = rpimemmgr_free_by_usraddr(sums_usr, &mgr);
  if (err)
    return err;
  
  return 0;
}
