.text
.globl _start
.align 1
_start:
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
