/*
 * integral.h - scalar integrals for PJFry library
 *
 * Valery Yundin <valery.yundin@desy.de>
 */

#ifndef QUL_INTEGRAL_H
#define QUL_INTEGRAL_H

#include "common.h"
#include "kinem.h"
#include "cache.h"

// #define USE_ONELOOP 1
// #define USE_QCDLOOP 1

class Initialize
{
  public:
    Initialize();
};

ICache::Ival qlI1(const Kinem1& k);
ICache::Ival qlI2(const Kinem2& k);
ICache::Ival qlI3(const Kinem3& k);
ICache::Ival qlI4(const Kinem4& k);

#ifdef USE_QCDLOOP
# ifdef HAVE_QCDLOOP1_H
#   include <qcdloop1.h>
# else
  extern "C" {
    void qlinit_();
    std::complex<double> F77_FUNC(qli1,QLI1)(double *m1, double *mu2, int *ep);
    std::complex<double> F77_FUNC(qli2,QLI2)(double *p1, double *m1, double *m2, double *mu2, int *ep);
    std::complex<double> F77_FUNC(qli3,QLI3)(double *p1, double *p2, double *p3, double *m1, double *m2, double *m3, double *mu2, int *ep);
    std::complex<double> F77_FUNC(qli4,QLI4)(double *p1, double *p2, double *p3, double *p4, double *s12, double *s23, double *m1, double *m2, double *m3, double *m4, double *mu2, int *ep);

    extern struct {
      double xloss,precx,precc,xalogm,xclogm,xalog2,xclog2,reqprc;
    } F77_FUNC(ffprec,FFPREC);
  }
#endif /* HAVE_QCDLOOP1_H */
#endif /* USE_QCDLOOP */

#ifdef USE_ONELOOP
  extern "C" {
    void F77_FUNC_(avh_olo_mu_set,AVH_OLO_MU_SET)(double *mu);
    void F77_FUNC_(avh_olo_onshell,AVH_OLO_ONSHELL)(double *thrs);
    void F77_FUNC_(avh_olo_a0m,AVH_OLO_A0M)(std::complex<double> *rslt, double *m1);
    void F77_FUNC_(avh_olo_b0m,AVH_OLO_B0M)(std::complex<double> *rslt, double *p1, double *m1, double *m2);
    void F77_FUNC_(avh_olo_c0m,AVH_OLO_C0M)(std::complex<double> *rslt, double *p1, double *p2, double *p3, double *m1, double *m2, double *m3);
    void F77_FUNC_(avh_olo_d0m,AVH_OLO_D0M)(std::complex<double> *rslt, double *p1, double *p2, double *p3, double *p4, double *p12, double *p23, double *m1, double *m2, double *m3, double *m4);
  }
#endif /* USE_ONELOOP */

#endif /* QUL_INTEGRAL_H */
