#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./gwnum/giants.h"

#define  ULONG unsigned long
#define MAXULONG 0xFFFFFF00

void trace(int);

/************************** Giants.c extensions *************************************/

#define BITSINULONG 32

int gmodi (	/* Returns g%i as an integer */ 
	ULONG i, giant g) { 
	ULONG wordweight, j, k, size, value;
	int sign;
	if (i==1 || g->sign==0) return 0;
	if (g->sign < 0) {
	    sign = -1;
	    size = -g->sign;
	}
	else {
	    sign = 1;
	    size = g->sign;
	}
	wordweight = 1;
	value = 0;
	for (j=0; j<size; j++) {
	    value += ((__int64)(g->n[j]%i)*wordweight)%i;
	    if (value >= i) value -= i;
	    for (k=1; k<=BITSINULONG; k++) {
		wordweight <<=1;
		if (wordweight >= i) wordweight -= i;
	    }
	}
	return sign*value;
}

void uldivg (ULONG den, giant num) {
	ULONG denval = den;
	giantstruct gdenstruct = {1, &denval};
	giant gden = &gdenstruct;
	divg (gden, num);
}

/************************************************************************************/

void Reduce (ULONG x, ULONG *d, ULONG *b) {

// Reduce a Discriminant to a square free integer

	ULONG div, sq;

	*d = x;
	*b = 1;

	while (!((*d)%4)) {
		*d /= 4;
		*b *= 2;
	}

	for (div = 3; (sq = div*div) <= *d; div += 2)
		while (!((*d)%sq)) {
			*d /= sq;
			*b *= div;
		}
}

ULONG issquare (ULONG n) {
	ULONG s;
	s = (ULONG)floor(sqrt(n));
	if (s*s == n)
		return s;
	else
		return 0;
}

ULONG twopownmodm (ULONG n, ULONG m, ULONG *order, ULONG *nmodorder) {
	ULONG tpnmodm, tp, i;
	tpnmodm = 0;
	*order = tp = 1;
	for (i=1; i<m; i++) {
	    tp <<= 1;
	    if (tp >= m) tp -= m;
	    if (i==n) tpnmodm = tp;
	    if (tp==1) {
		*order = i;
		break;
	    }
	}
	*nmodorder = n%*order;
	if (!tpnmodm) {
	    for (i=1, tp=1; i<=*nmodorder; i++) {
		tp <<= 1;
		if (tp >= m) tp -= m;
	    }
	    tpnmodm = tp;
	}
	return tpnmodm;
}

ULONG Bachet(ULONG u, ULONG v, long *a, long *b) {

//  Computes a and b such as a*u+b*v = gcd(u,v),
//  returns gcd(u,v).

    ULONG n=0, m11=1, m12=0, m21=0, m22=1;
    ULONG q, newm11, newm21, newu;

    while (v!=0) {
	q = u/v;
	newm11 = m11*q+m12;
	m12 = m11;
	m11 = newm11;
	newm21 = m21*q+m22;
	m22 = m21;
	m21 = newm21;
	newu = v;
	v = u-q*v;
	u = newu;
	n++;
    }
    if (n&1) {
	*a = -(int)m22;
	*b = (int)m12;
    }
    else {
	*a = (int)m22;
	*b = -(int)m12;
    }
    return u;
}

int gen_v1(giant k, ULONG n, int debug, ULONG maxres) {
    long sign, j2N, jNd, jNa, v, vcand;
    long idred, iared, iorderd, iordera, kdiff, ndiff;
    ULONG nbres, kmod3, rawd, d, dred, kmodd, tpnmd, i, orderd, Nmodd;
    ULONG V, W, dsqW, aplus, aminus, b, bcand, rplus, rminus, found, iseps2;
    ULONG nmodorderd, ared, kmoda, tpnma, ordera, nmodordera, Nmoda;
    ULONG dk, dn, kmod, nmod, modulk, moduln;
    nbres = 0;
    if (n>2)
	j2N = 1;
    else
	j2N = -1;
    kmod3 = gmodi (3, k);
    if (maxres == 1 && kmod3)		// Consider only the simple case !
	if ((kmod3 == 1 && !(n&1)) || (kmod3 == 2 && (n&1))) {
	    if (debug)
		printf ("d = 3 divides N !\n");
	    return (-3);
	}
	else {
	    if (debug) {
		printf ("epsilon = 2+sqrt(3)\n");
		printf ("k = %d (mod 3), n = %d (mod 2)\n", kmod3, n&1);
		printf ("v1 = 4, d = 3, a = 6, b = 2, r = 24, +1\n");
		printf ("v1 = 4, d = 3, a = 2, b = 2, r = 8, -1\n");
	    }
	    return (4);
	}
    for (v=3; (rawd = v*v-4)<MAXULONG; v++) {
	Reduce (rawd, &d, &b);
	if (!(d&1)) {
	    sign = j2N;
	    dred = d>>1;
	}
	else {
	    sign = 1;
	    dred = d;
	}
	if (d==2) {
	    if (n > 2) continue;
	}
	else {
	  kmodd = gmodi (dred, k);
	  if (!kmodd) continue;
	  if (n>1 && (((dred-1)/2) & 1)) sign = - sign;
	  tpnmd = twopownmodm (n, dred, &orderd, &nmodorderd);
	  Nmodd = (kmodd*tpnmd-1)%dred;
	  if (!Nmodd) {
	    if (debug)
		if (d&1)
		    printf ("d = %d divides N !\n", dred);
		else
		    printf ("d/2 = %d divides N !\n", dred);
	    return (-(int)dred);
	  }
	  if ((jNd = jacobi(Nmodd,dred)) > 1) {
	    if (debug)
		printf ("%d divides d = %d and N !\n", jNd, d);
	    return (-jNd);
	  }
	  if ((sign*jNd) != -1) continue;
	}
	found = 0;
	for (W=1; (dsqW=d*W*W) < MAXULONG; W++)
	    if (dsqW>4 && (V=issquare(dsqW-4))) {
		vcand = dsqW-2;
		bcand = V*W;
		iseps2 = 1;
		found = 1;
		break;
	    }
	    else if (V=issquare(dsqW+4)) {
		vcand = V;
		bcand = W;
		iseps2 = 0;
		found = 1;
		break;
	    }
	if (found && v==vcand && b==bcand) {
	    aplus = v+2;
	    aminus = v-2;
	    rplus = 4*aplus;
	    rminus = 4*aminus;
	    if (iseps2) {
		nbres++;
		if (debug) {
		    if (V&1 || W&1)
			if (W != 1)
			    printf ("epsilon = [%d+%d*sqrt(%d)]/2\n", V, W, d);
			else
			    printf ("epsilon = [%d+sqrt(%d)]/2\n", V, d);
		    else
			if (W/2 != 1)
			    printf ("epsilon = %d+%d*sqrt(%d)\n", V/2, W/2, d);
			else
			    printf ("epsilon = %d+sqrt(%d)\n", V/2, d);
		    printf ("k = %d (mod %d), n = %d (mod %d)\n", kmodd, dred, nmodorderd, orderd);
		    printf ("v1 = %d, d = %d, a = %d, b = %d, r = %d, +1,eps2\n",
				v, d, aplus, b, rplus);
		    printf ("v1 = %d, d = %d, a = %d, b = %d, r = %d, -1,eps2\n",
				v, d, aminus, b, rminus);
		}
		if (nbres == maxres) return v;
	    }
	    else {
		for (ared=aplus, i=0; !(ared & 1); ared >>= 1) i++;
		if (i&1)
		    sign = j2N;
		else
		    sign = 1;
		kmoda = gmodi (ared, k);
		if (!kmoda) continue;
		if (n>1 && (((ared-1)/2) & 1)) sign = - sign;
		tpnma = twopownmodm (n, ared, &ordera, &nmodordera);
		Nmoda = (kmoda*tpnma-1)%ared;
		if (!Nmoda) {
	 	   if (debug)
			printf ("a/%d = %d divides N !\n", aplus/ared, ared);
		    return (-(int)ared);
		}
		if ((jNa = jacobi(Nmoda,ared)) > 1) {
		    if (debug)
			printf ("%d divides a = %d and N !\n", jNa, aplus);
		    return (-jNa);
		}
		if ((sign*jNa) != -1) continue;
		nbres++;
		if (debug) {
		    if (V&1 || W&1)
			if (W != 1)
			    printf ("epsilon = [%d+%d*sqrt(%d)]/2\n", V, W, d);
			else
			    printf ("epsilon = [%d+sqrt(%d)]/2\n", V, d);
		    else
			if (W/2 != 1)
			    printf ("epsilon = %d+%d*sqrt(%d)\n", V/2, W/2, d);
			else
			    printf ("epsilon = %d+sqrt(%d)\n", V/2, d);
		    dk = Bachet (dred, ared, &idred, &iared);
		    modulk = dred*(ared/dk);		// lcm(dred, ared).
		    kdiff = kmoda-kmodd;
		    if (kdiff >=0) {
			while (idred<0)
			    idred += ared;
			kmod = (kmodd + idred*(kdiff/dk)*dred)%modulk;
		    }
		    else {
			kdiff = -kdiff;
			while (iared<0)
			    iared += dred;
			kmod = (kmoda + iared*(kdiff/dk)*ared)%modulk;
		    }
		    dn = Bachet (orderd, ordera, &iorderd, &iordera);
		    moduln = orderd*(ordera/dn);	// lcm(orderd, ordera).
		    ndiff = nmodordera-nmodorderd;
		    if (ndiff >=0) {
			while (iorderd<0)
			    iorderd += ordera;
			nmod = (nmodorderd + iorderd*(ndiff/dn)*orderd)%moduln;
		    }
		    else {
			ndiff = -ndiff;
			while (iordera<0)
			    iordera += orderd;
			nmod = (nmodordera + iordera*(ndiff/dn)*ordera)%moduln;
		    }
		    printf ("k = %d (mod %d), n = %d (mod %d)\n", kmod, modulk, nmod, moduln);
		    printf ("v1 = %d, d = %d, a = %d, b = %d, r = %d, +1\n",
				v, d, aplus, b, rplus);
		    printf ("v1 = %d, d = %d, a = %d, b = %d, r = %d, -1\n",
				v, d, aminus, b, rminus);
		}
		if (nbres == maxres) return v;
	    }
	}
    }
    return -1;		// Unable to find a value for v...
}

int genProthBase(giant k, ULONG n) {
	ULONG Nmodp, kmodp, p, tpnmp, orderp, nmodorderp, kw;
	int jNp;

//	Return the least prime p such Jacobi (N, p) = -1

	if (k->sign == 1 && n < 3) {	//	Eliminate some trivial cases
		kw = k->n[0];
		if (n == 1 && kw == 1)
			return (2);
		else if (n == 2)
			return (2);
		else
			return (-1);
	}
	else {							// General case
		for (p = 3; p<=2147483647; p+=2) {
			if (!isPrime(p))
				continue;
			kmodp = gmodi (p, k);
			if (!kmodp)
				continue;
			tpnmp = twopownmodm (n, p, &orderp, &nmodorderp);
			Nmodp = (kmodp*tpnmp+1)%p;
			if (!Nmodp) {
				return (-(int)p);
			}
			if ((jNp = jacobi(Nmodp, p)) > 1) {
				return (-jNp);
			}
			if (jNp != -1)
				continue;
			return (p);
		}
		return (-1);
	}
}
