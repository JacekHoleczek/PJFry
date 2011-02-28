/*
 * minorex.cpp - extra integrals for asymptotic expansion for PJFry library
 *
 * Valery Yundin <valery.yundin@desy.de>
 */

#include "minor.h"
#include "cache.h"

/* ========================================================
 *               SMALL Gram4 expansion
 * ========================================================
 */

/* --------------------------------------------------------
 *   I2D3stu bubble in D+6 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I2D3stu(int ep, int s, int t, int u)
{
  assert(t!=u && u!=s && s!=t); //if (t==u || u==s || s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I2D3stu+ep]) {
    I2D3stuEval(0,ep,1,2,3,4,5,kinem.p5());
    I2D3stuEval(1,ep,1,2,4,3,5,kinem.s45());
    I2D3stuEval(2,ep,1,2,5,3,4,kinem.p4());

    I2D3stuEval(3,ep,1,3,4,2,5,kinem.s12());
    I2D3stuEval(4,ep,1,3,5,2,4,kinem.s34());

    I2D3stuEval(5,ep,1,4,5,2,3,kinem.p3());

    if (smax==5) {
      I2D3stuEval(6,ep,2,3,4,1,5,kinem.p1());
      I2D3stuEval(7,ep,2,3,5,1,4,kinem.s15());

      I2D3stuEval(8,ep,2,4,5,1,3,kinem.s23());


      I2D3stuEval(9,ep,3,4,5,1,2,kinem.p2());
    }

    fEval[E_I2D3stu+ep]=true;
  }
  int tmp;
  tswap(s,u,tmp);
  tswap(t,u,tmp);
  tswap(s,t,tmp);
  int idx=im3(s,t,u)-10;
  return pI2D3stu[ep][idx];
}

void Minor5::I2D3stuEval(int idx, int ep, int s, int t, int u, int m, int n, double qsq)
{
  ncomplex sum1=0;
  if (ep==0) {
    const double dstustu=-2*qsq; /*M3(s,t,u,s,t,u);*/
    const double msq1=kinem.mass(m);
    const double msq2=kinem.mass(n);

    if (fabs(dstustu) <= deps) {
      const double mm12=msq1-msq2;
      if (fabs(mm12) < meps) {
        sum1=-(5*msq1 + 6.*ICache::getI1(ep, Kinem1(msq1)))*msq1*msq1/36.;
      }
      else {
        sum1= -13*((msq1+msq2)*(msq1*msq1+msq2*msq2))/288
              + ( - msq1*msq1*msq1*ICache::getI1(ep, Kinem1(msq1))
                  + msq2*msq2*msq2*ICache::getI1(ep, Kinem1(msq2))
                  )/(24*mm12);
      }
    }
    else {
      ncomplex sumX=7.*I2D2stu(ep,s,t,u)+2.*I2D2stu(ep+1,s,t,u);
      ncomplex sumY=(42.*ICache::getI1(ep, Kinem1(msq2))+47*msq2)*msq2*msq2/36.;
      ncomplex sumZ=(42.*ICache::getI1(ep, Kinem1(msq1))+47*msq1)*msq1*msq1/36.;

      const double ds0tu=(Cay[iss(m,m)]*Cay[iss(n,n)]-Cay[iss(m,n)]*Cay[iss(m,n)]);
      sum1+=sumX*ds0tu;

      const double dsvtuY=-(Cay[iss(n,n)]-Cay[iss(m,n)]); /* minus sign of minor v=m */
      sum1-=sumY*dsvtuY;

      const double dsvtuZ=+(Cay[iss(m,n)]-Cay[iss(m,m)]); /* plus sign of minor v=n */
      sum1-=sumZ*dsvtuZ;

      sum1/=49*dstustu;
    }
  }
  else { assert(ep==1);
    const double y11=Cay[iss(m,m)];
    const double y12=Cay[iss(m,n)];
    const double y22=Cay[iss(n,n)];
    sum1=-(+ y11*y11*(y22 + 5*(y11 + y12))
            + y22*y22*(y11 + 5*(y22 + y12))
            + 2*y12*y12*(y12 + 2*(y11 + y22))
            + 3*y11*y22*y12
            )/1680;
  }
  pI2D3stu[ep][idx]=sum1;
}

/* --------------------------------------------------------
 *   I3D4st triangle in D+8 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I3D4st(int ep, int s, int t)
{
  assert(s!=t); //if (s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I3D4st+ep]) {
    I3D4stEval(ep);
  }
  int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;
  return pI3D4st[ep][idx];
}

void Minor5::I3D4stEval(int ep)
{
  for (int s=1; s<=smax; s++) {
    for (int t=s+1; t<=5; t++) {
      int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;

      const double dstst=M2(s,t,s,t);
      ncomplex ivalue=0;

      if (maxS3(s,t) <= deps) {
        printf("I3D4 - M2({%d,%d}) <= eps\n",s,t);
        ivalue=std::numeric_limits<double>::quiet_NaN();
      }
      else {
        double cf=1./8.;
        for (int ei=ep; ei<=1; ei++) {
          ncomplex sum1=M3(0,s,t,0,s,t)*I3D3st(ei, s, t);
          for (int u=1; u<=5; u++) {
            if (t==u || s==u) continue;
            sum1-=M3(u,s,t,0,s,t)*I2D3stu(ei, s, t, u);
          }
          ivalue+=cf*sum1;
          cf*=1./4.;
        }
        ivalue/=dstst;
      }
      pI3D4st[ep][idx]=ivalue;
    }
  }
  fEval[E_I3D4st+ep]=true;
}


/* --------------------------------------------------------
 *   I2D4stu bubble in D+8 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I2D4stu(int ep, int s, int t, int u)
{
  assert(t!=u && u!=s && s!=t); //if (t==u || u==s || s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I2D4stu+ep]) {
    I2D4stuEval(0,ep,1,2,3,4,5,kinem.p5());
    I2D4stuEval(1,ep,1,2,4,3,5,kinem.s45());
    I2D4stuEval(2,ep,1,2,5,3,4,kinem.p4());

    I2D4stuEval(3,ep,1,3,4,2,5,kinem.s12());
    I2D4stuEval(4,ep,1,3,5,2,4,kinem.s34());

    I2D4stuEval(5,ep,1,4,5,2,3,kinem.p3());

    if (smax==5) {
      I2D4stuEval(6,ep,2,3,4,1,5,kinem.p1());
      I2D4stuEval(7,ep,2,3,5,1,4,kinem.s15());

      I2D4stuEval(8,ep,2,4,5,1,3,kinem.s23());


      I2D4stuEval(9,ep,3,4,5,1,2,kinem.p2());
    }

    fEval[E_I2D4stu+ep]=true;
  }
  int tmp;
  tswap(s,u,tmp);
  tswap(t,u,tmp);
  tswap(s,t,tmp);
  int idx=im3(s,t,u)-10;
  return pI2D4stu[ep][idx];
}

void Minor5::I2D4stuEval(int idx, int ep, int s, int t, int u, int m, int n, double qsq)
{
  ncomplex sum1=0;
  if (ep==0) {
    const double dstustu=-2*qsq; /*M3(s,t,u,s,t,u);*/
    const double msq1=kinem.mass(m);
    const double msq2=kinem.mass(n);

    if (fabs(dstustu) <= deps) {
      const double mm12=msq1-msq2;
      if (fabs(mm12) < meps) {
        sum1=(13*msq1 + 12.*ICache::getI1(ep, Kinem1(msq1)))*msq1*msq1*msq1/288.;
      }
      else {
        const double msq1p4=(msq1*msq1)*(msq1*msq1);
        const double msq2p4=(msq2*msq2)*(msq2*msq2);
        sum1= (77*(msq1*msq1p4-msq2*msq2p4)/7200
              + ( + msq1p4*ICache::getI1(ep, Kinem1(msq1))
                  - msq2p4*ICache::getI1(ep, Kinem1(msq2))
                  )/120.)/mm12;
      }
    }
    else {
      ncomplex sumX=9.*I2D3stu(ep,s,t,u)+2.*I2D3stu(ep+1,s,t,u);
      ncomplex sumY=-(36.*ICache::getI1(ep, Kinem1(msq2))+47*msq2)*msq2*msq2*msq2/96.;
      ncomplex sumZ=-(36.*ICache::getI1(ep, Kinem1(msq1))+47*msq1)*msq1*msq1*msq1/96.;

      const double ds0tu=(Cay[iss(m,m)]*Cay[iss(n,n)]-Cay[iss(m,n)]*Cay[iss(m,n)]);
      sum1+=sumX*ds0tu;

      const double dsvtuY=-(Cay[iss(n,n)]-Cay[iss(m,n)]); /* minus sign of minor v=m */
      sum1-=sumY*dsvtuY;

      const double dsvtuZ=+(Cay[iss(m,n)]-Cay[iss(m,m)]); /* plus sign of minor v=n */
      sum1-=sumZ*dsvtuZ;

      sum1/=81*dstustu;
    }
    /* printf("I2D4stu(%e,%e,%e)^%d = %e,%e\n",-0.5*dstustu,msq1,msq2,ep,sum1.real(),sum1.imag());
    */
  }
  else { assert(ep==1);
    const double y11=Cay[iss(m,m)];
    const double y12=Cay[iss(m,n)];
    const double y22=Cay[iss(n,n)];
    sum1=(
          +y11*y11*(y11*(35*(y11+y12)+5*y22)+15*y12*(2*y12+y22))
          +y22*y22*(y22*(35*(y22+y12)+5*y11)+15*y12*(2*y12+y11))
          +y12*y12*(y12*(8*y12+20*y11+20*y22)+24*y11*y22)
          +3*y11*y11*y22*y22
            )/120960;
  }
  pI2D4stu[ep][idx]=sum1;
}

/* --------------------------------------------------------
 *   I3D5st triangle in D+10 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I3D5st(int ep, int s, int t)
{
  assert(s!=t); //if (s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I3D5st+ep]) {
    I3D5stEval(ep);
  }
  int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;
  return pI3D5st[ep][idx];
}

void Minor5::I3D5stEval(int ep)
{
  for (int s=1; s<=smax; s++) {
    for (int t=s+1; t<=5; t++) {
      int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;

      const double dstst=M2(s,t,s,t);
      ncomplex ivalue=0;

      if (maxS3(s,t) <= deps) {
        printf("I3D5 - M2({%d,%d}) <= eps\n",s,t);
        ivalue=std::numeric_limits<double>::quiet_NaN();
      }
      else {
        double cf=1./10.;
        for (int ei=ep; ei<=1; ei++) {
          ncomplex sum1=M3(0,s,t,0,s,t)*I3D4st(ei, s, t);
          for (int u=1; u<=5; u++) {
            if (t==u || s==u) continue;
            sum1-=M3(u,s,t,0,s,t)*I2D4stu(ei, s, t, u);
          }
          ivalue+=cf*sum1;
          cf*=1./5.;
        }
        ivalue/=dstst;
      }
      pI3D5st[ep][idx]=ivalue;
    }
  }
  fEval[E_I3D5st+ep]=true;
}

/* --------------------------------------------------------
 *   I2D5stu bubble in D+10 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I2D5stu(int ep, int s, int t, int u)
{
  assert(t!=u && u!=s && s!=t); //if (t==u || u==s || s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I2D5stu+ep]) {
    I2D5stuEval(0,ep,1,2,3,4,5,kinem.p5());
    I2D5stuEval(1,ep,1,2,4,3,5,kinem.s45());
    I2D5stuEval(2,ep,1,2,5,3,4,kinem.p4());

    I2D5stuEval(3,ep,1,3,4,2,5,kinem.s12());
    I2D5stuEval(4,ep,1,3,5,2,4,kinem.s34());

    I2D5stuEval(5,ep,1,4,5,2,3,kinem.p3());

    if (smax==5) {
      I2D5stuEval(6,ep,2,3,4,1,5,kinem.p1());
      I2D5stuEval(7,ep,2,3,5,1,4,kinem.s15());

      I2D5stuEval(8,ep,2,4,5,1,3,kinem.s23());


      I2D5stuEval(9,ep,3,4,5,1,2,kinem.p2());
      }

    fEval[E_I2D5stu+ep]=true;
  }
  int tmp;
  tswap(s,u,tmp);
  tswap(t,u,tmp);
  tswap(s,t,tmp);
  int idx=im3(s,t,u)-10;
  return pI2D5stu[ep][idx];
}

void Minor5::I2D5stuEval(int idx, int ep, int s, int t, int u, int m, int n, double qsq)
{
  ncomplex sum1=0;
  if (ep==0) {
    const double dstustu=-2*qsq; /*M3(s,t,u,s,t,u);*/
    const double msq1=kinem.mass(m);
    const double msq2=kinem.mass(n);

    if (fabs(dstustu) <= deps) {
      const double mm12=msq1-msq2;
      if (fabs(mm12) < meps) {
        sum1=-(77*msq1 + 60.*ICache::getI1(ep, Kinem1(msq1)))*(msq1*msq1)*(msq1*msq1)/7200.;
      }
      else {
        const double msq1p5=(msq1*msq1)*(msq1*msq1)*msq1;
        const double msq2p5=(msq2*msq2)*(msq2*msq2)*msq2;
        sum1=-(29*(msq1*msq1p5-msq2*msq2p5)/14400
              + ( + msq1p5*ICache::getI1(ep, Kinem1(msq1))
                  - msq2p5*ICache::getI1(ep, Kinem1(msq2))
                  )/720.)/mm12;
      }
    }
    else {
      ncomplex sumX=11.*I2D4stu(ep,s,t,u)+2.*I2D4stu(ep+1,s,t,u);
      ncomplex sumY=(660.*ICache::getI1(ep, Kinem1(msq2))+967*msq2)*(msq2*msq2)*(msq2*msq2)/7200.;
      ncomplex sumZ=(660.*ICache::getI1(ep, Kinem1(msq1))+967*msq1)*(msq1*msq1)*(msq1*msq1)/7200.;

      const double ds0tu=(Cay[iss(m,m)]*Cay[iss(n,n)]-Cay[iss(m,n)]*Cay[iss(m,n)]);
      sum1+=sumX*ds0tu;

      const double dsvtuY=-(Cay[iss(n,n)]-Cay[iss(m,n)]); /* minus sign of minor v=m */
      sum1-=sumY*dsvtuY;

      const double dsvtuZ=+(Cay[iss(m,n)]-Cay[iss(m,m)]); /* plus sign of minor v=n */
      sum1-=sumZ*dsvtuZ;

      sum1/=121*dstustu;
    }
  }
  else { assert(ep==1);
    const double y11=Cay[iss(m,m)];
    const double y12=Cay[iss(m,n)];
    const double y22=Cay[iss(n,n)];
    sum1=-(
            y11*y11*y11*(y11*(63*(y11+y12)+7*y22)+7*y12*(8*y12+3*y22)+3*y22*y22)
          + y22*y22*y22*(y22*(63*(y22+y12)+7*y11)+7*y12*(8*y12+3*y11)+3*y11*y11)
          + y12*y12*y12*(8*y12*(y12+3*y11+3*y22)+6*(7*y11*y11+4*y11*y22+7*y22*y22))
          + y11*y22*y12*(4*y12*(9*(y11+y22)+4*y12)+15*y11*y22)
            )/2661120;
  }
  pI2D5stu[ep][idx]=sum1;
}

/* --------------------------------------------------------
 *   I3D6st triangle in D+12 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I3D6st(int ep, int s, int t)
{
  assert(s!=t); //if (s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I3D6st+ep]) {
    I3D6stEval(ep);
  }
  int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;
  return pI3D6st[ep][idx];
}

void Minor5::I3D6stEval(int ep)
{
  for (int s=1; s<=smax; s++) {
    for (int t=s+1; t<=5; t++) {
      int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;

      const double dstst=M2(s,t,s,t);
      ncomplex ivalue=0;

      if (maxS3(s,t) <= deps) {
        printf("I3D6 - M2({%d,%d}) <= eps\n",s,t);
        ivalue=std::numeric_limits<double>::quiet_NaN();
      }
      else {
        double cf=1./12.;
        for (int ei=ep; ei<=1; ei++) {
          ncomplex sum1=M3(0,s,t,0,s,t)*I3D5st(ei, s, t);
          for (int u=1; u<=5; u++) {
            if (t==u || s==u) continue;
            sum1-=M3(u,s,t,0,s,t)*I2D5stu(ei, s, t, u);
          }
          ivalue+=cf*sum1;
          cf*=1./6.;
        }
        ivalue/=dstst;
      }
      pI3D6st[ep][idx]=ivalue;
    }
  }
  fEval[E_I3D6st+ep]=true;
}

/* --------------------------------------------------------
 *   I2D6stu bubble in D+12 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I2D6stu(int ep, int s, int t, int u)
{
  assert(t!=u && u!=s && s!=t); //if (t==u || u==s || s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I2D6stu+ep]) {
    I2D6stuEval(0,ep,1,2,3,4,5,kinem.p5());
    I2D6stuEval(1,ep,1,2,4,3,5,kinem.s45());
    I2D6stuEval(2,ep,1,2,5,3,4,kinem.p4());

    I2D6stuEval(3,ep,1,3,4,2,5,kinem.s12());
    I2D6stuEval(4,ep,1,3,5,2,4,kinem.s34());

    I2D6stuEval(5,ep,1,4,5,2,3,kinem.p3());

    if (smax==5) {
      I2D6stuEval(6,ep,2,3,4,1,5,kinem.p1());
      I2D6stuEval(7,ep,2,3,5,1,4,kinem.s15());

      I2D6stuEval(8,ep,2,4,5,1,3,kinem.s23());


      I2D6stuEval(9,ep,3,4,5,1,2,kinem.p2());
    }

    fEval[E_I2D6stu+ep]=true;
  }
  int tmp;
  tswap(s,u,tmp);
  tswap(t,u,tmp);
  tswap(s,t,tmp);
  int idx=im3(s,t,u)-10;
  return pI2D6stu[ep][idx];
}

void Minor5::I2D6stuEval(int idx, int ep, int s, int t, int u, int m, int n, double qsq)
{
  ncomplex sum1=0;
  if (ep==0) {
    const double dstustu=-2*qsq; /*M3(s,t,u,s,t,u);*/
    const double msq1=kinem.mass(m);
    const double msq2=kinem.mass(n);

    if (fabs(dstustu) <= deps) {
      const double mm12=msq1-msq2;
      if (fabs(mm12) < meps) {
        sum1=(29*msq1 + 20.*ICache::getI1(ep, Kinem1(msq1)))*(msq1*msq1)*(msq1*msq1)*msq1/14400.;
      }
      else {
        const double msq1p6=(msq1*msq1)*(msq1*msq1)*(msq1*msq1);
        const double msq2p6=(msq2*msq2)*(msq2*msq2)*(msq2*msq2);
        sum1=(223*(msq1*msq1p6-msq2*msq2p6)/705600
              + ( + msq1p6*ICache::getI1(ep, Kinem1(msq1))
                  - msq2p6*ICache::getI1(ep, Kinem1(msq2))
                  )/5040.)/mm12;
      }
    }
    else {
      ncomplex sumX=13.*I2D5stu(ep,s,t,u)+2.*I2D5stu(ep+1,s,t,u);
      ncomplex sumY=-(260.*ICache::getI1(ep, Kinem1(msq2))+417*msq2)*(msq2*msq2)*(msq2*msq2)*msq2/14400.;
      ncomplex sumZ=-(260.*ICache::getI1(ep, Kinem1(msq1))+417*msq1)*(msq1*msq1)*(msq1*msq1)*msq1/14400.;

      const double ds0tu=(Cay[iss(m,m)]*Cay[iss(n,n)]-Cay[iss(m,n)]*Cay[iss(m,n)]);
      sum1+=sumX*ds0tu;

      const double dsvtuY=-(Cay[iss(n,n)]-Cay[iss(m,n)]); /* minus sign of minor v=m */
      sum1-=sumY*dsvtuY;

      const double dsvtuZ=+(Cay[iss(m,n)]-Cay[iss(m,m)]); /* plus sign of minor v=n */
      sum1-=sumZ*dsvtuZ;

      sum1/=169*dstustu;
    }
  }
  else { assert(ep==1);
    const double y11=Cay[iss(m,m)];
    const double y12=Cay[iss(m,n)];
    const double y22=Cay[iss(n,n)];
    sum1=(
      y11*y11*y11*(y11*(21*y11*(11*(y11+y12)+y22)+210*y12*y12+7*y22*y22+63*y22*y12)
      +y12*y12*(168*y12+112*y22))+y22*y22*y22*(y22*(21*y22*(11*(y22
      +y12)+y11)+210*y12*y12+7*y11*y11+63*y11*y12)+y12*y12*(168*y12+112*y11))
      +y12*y12*(y12*y12*(16*y12*y12+112*(y11*y11+y22*y22)+56*y12*(y11+y22)
      +120*y11*y22)+y11*y22*(90*y11*y22+140*y12*(y22+y11)))+y11*y11*y22*y22*(35*(y11+y22)*y12+5*y11*y22)
      )/138378240;
  }
  pI2D6stu[ep][idx]=sum1;
}

/* --------------------------------------------------------
 *   I3D7st triangle in D+14 dim
 * --------------------------------------------------------
 */
ncomplex Minor5::I3D7st(int ep, int s, int t)
{
  assert(s!=t); //if (s==t) return 0;
  if (ep==2) return 0;
  if (not fEval[E_I3D7st+ep]) {
    I3D7stEval(ep);
  }
  int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;
  return pI3D7st[ep][idx];
}

void Minor5::I3D7stEval(int ep)
{
  for (int s=1; s<=smax; s++) {
    for (int t=s+1; t<=5; t++) {
      int idx = s<t ? im2(s,t)-5 : im2(t,s)-5;

      const double dstst=M2(s,t,s,t);
      ncomplex ivalue=0;

      if (maxS3(s,t) <= deps) {
        printf("I3D7 - M2({%d,%d}) <= eps\n",s,t);
        ivalue=std::numeric_limits<double>::quiet_NaN();
      }
      else {
        double cf=1./14.;
        for (int ei=ep; ei<=1; ei++) {
          ncomplex sum1=M3(0,s,t,0,s,t)*I3D6st(ei, s, t);
          for (int u=1; u<=5; u++) {
            if (t==u || s==u) continue;
            sum1-=M3(u,s,t,0,s,t)*I2D6stu(ei, s, t, u);
          }
          ivalue+=cf*sum1;
          cf*=1./7.;
        }
        ivalue/=dstst;
      }
      pI3D7st[ep][idx]=ivalue;
    }
  }
  fEval[E_I3D7st+ep]=true;
}

