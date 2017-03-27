#include "TrackingPlane.h"

namespace recob {
  namespace tracking {

    // formulas for 5->6 and 6->5 transformations
    //
    // par6d[0] = planePos.X() + par5d[0]*cosalpha;
    // par6d[1] = planePos.Y() + par5d[0]*sinalpha*sinbeta + par5d[1]*cosbeta;
    // par6d[2] = planePos.Z() - par5d[0]*sinalpha*cosbeta + par5d[1]*sinbeta;
    // par6d[3] = (par5d[2]*cosalpha + 1.*sinalpha)/(par5d[4]*std::sqrt(1. + par5d[2]*par5d[2] + par5d[3]*par5d[3]));
    // par6d[4] = (par5d[2]*sinalpha*sinbeta + par5d[3]*cosbeta - cosalpha*sinbeta)/(par5d[4]*std::sqrt(1. + par5d[2]*par5d[2] + par5d[3]*par5d[3]));
    // par6d[5] = (-par5d[2]*sinalpha*cosbeta + par5d[3]*sinbeta + cosalpha*cosbeta)/(par5d[4]*std::sqrt(1. + par5d[2]*par5d[2] + par5d[3]*par5d[3]));
    //
    // par5d[0] = (par6d[0]-planePos.X())*cosalpha + (par6d[1]-planePos.Y())*sinalpha*sinbeta - (par6d[2]-planePos.Z())*sinalpha*cosbeta;
    // par5d[1] = (par6d[1]-planePos.Y())*cosbeta + (par6d[2]-planePos.Z())*sinbeta;
    // par5d[2] = (par6d[3]*cosalpha + par6d[4]*sinalpha*sinbeta - par6d[5]*sinalpha*cosbeta)/(par6d[3]*sinalpha - par6d[4]*cosalpha*sinbeta + par6d[5]*cosalpha*cosbeta);
    // par5d[3] = (par6d[4]*cosbeta + par6d[5]*sinbeta)/(par6d[3]*sinalpha - par6d[4]*cosalpha*sinbeta + par6d[5]*cosalpha*cosbeta);
    // par5d[4] = 1./sqrt(par6d[3]*par6d[3]+par6d[4]*par6d[4]+par6d[5]*par6d[5]);

    SVector6 Plane::Local5DToGlobal6DParameters(const SVector5& par5d, const Point_t& planePos,const Vector_t& planeDir, const TrigCache& trigCache, bool trackAlongPlaneDir) const {
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      const double denom = (trackAlongPlaneDir ? par5d[4]*std::sqrt(1. + par5d[2]*par5d[2] + par5d[3]*par5d[3]) : -par5d[4]*std::sqrt(1. + par5d[2]*par5d[2] + par5d[3]*par5d[3]) );
      SVector6 par6d;
      par6d[0] = planePos.X() + par5d[0]*cosalpha;
      par6d[1] = planePos.Y() + par5d[0]*sinalpha*sinbeta + par5d[1]*cosbeta;
      par6d[2] = planePos.Z() - par5d[0]*sinalpha*cosbeta + par5d[1]*sinbeta;
      par6d[3] = (par5d[2]*cosalpha + 1.*sinalpha)/denom;
      par6d[4] = (par5d[2]*sinalpha*sinbeta + par5d[3]*cosbeta - cosalpha*sinbeta)/denom;
      par6d[5] = (-par5d[2]*sinalpha*cosbeta + par5d[3]*sinbeta + cosalpha*cosbeta)/denom;
      return par6d;
    }

    SVector5 Plane::Global6DToLocal5DParameters(const SVector6& par6d, const Point_t& planePos,const Vector_t& planeDir, const TrigCache& trigCache) const {
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      const double pu = par6d[3]*cosalpha + par6d[4]*sinalpha*sinbeta - par6d[5]*sinalpha*cosbeta;
      const double pv = par6d[4]*cosbeta + par6d[5]*sinbeta;
      const double pw = par6d[3]*sinalpha - par6d[4]*cosalpha*sinbeta + par6d[5]*cosalpha*cosbeta;
      const double pval = sqrt(par6d[3]*par6d[3]+par6d[4]*par6d[4]+par6d[5]*par6d[5]);
      SVector5 par5d;
      par5d[0] = (par6d[0]-planePos.X())*cosalpha + (par6d[1]-planePos.Y())*sinalpha*sinbeta - (par6d[2]-planePos.Z())*sinalpha*cosbeta;
      par5d[1] = (par6d[1]-planePos.Y())*cosbeta + (par6d[2]-planePos.Z())*sinbeta;
      par5d[2] = pu/pw;
      par5d[3] = pv/pw;
      par5d[4] = (pval>0 ? 1./pval : 1.);
      return par5d;
    }

    SMatrix65 Plane::Local5DToGlobal6DJacobian(bool hasMomentum, const Vector_t& trackMomOrDir, const Vector_t& planeDir, const TrigCache& trigCache) const {
      bool trackAlongPlaneDir = trackMomOrDir.Dot(planeDir)>0;
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      const double pu = trackMomOrDir.X()*cosalpha + trackMomOrDir.Y()*sinalpha*sinbeta - trackMomOrDir.Z()*sinalpha*cosbeta;
      const double pv = trackMomOrDir.Y()*cosbeta + trackMomOrDir.Z()*sinbeta;
      const double pw = trackMomOrDir.X()*sinalpha - trackMomOrDir.Y()*cosalpha*sinbeta + trackMomOrDir.Z()*cosalpha*cosbeta;
      //local parameters 2,3,4
      const double l2 = pu/pw;
      const double l3 = pv/pw;
      const double p2 = trackMomOrDir.X()*trackMomOrDir.X() + trackMomOrDir.Y()*trackMomOrDir.Y() + trackMomOrDir.Z()*trackMomOrDir.Z();
      const double l4 = (hasMomentum ? 1./sqrt(p2) : 1.);
      const double den23 = ( trackAlongPlaneDir ? l4*(l2*l2+l3*l3+1.)*sqrt(l2*l2+l3*l3+1.) : -l4*(l2*l2+l3*l3+1.)*sqrt(l2*l2+l3*l3+1.) );
      const double den4 = l4*l4*sqrt(l2*l2+l3*l3+1.);
      SMatrix65 j;
      //
      j(0,0) = cosalpha;
      j(0,1) = 0.;
      j(0,2) = 0.;
      j(0,3) = 0.;
      j(0,4) = 0.;
      //
      j(1,0) = sinalpha*sinbeta;
      j(1,1) = cosbeta;
      j(1,2) = 0.;
      j(1,3) = 0.;
      j(1,4) = 0.;
      //
      j(2,0) = -sinalpha*cosbeta;
      j(2,1) = sinbeta;
      j(2,2) = 0.;
      j(2,3) = 0.;
      j(2,4) = 0.;
      //
      j(3,0) = 0.;
      j(3,1) = 0.;
      j(3,2) = (cosalpha*(l3*l3+1.) - sinalpha*l2)/den23;
      j(3,3) = -l3*(l2*cosalpha + sinalpha)/den23;
      j(3,4) = (hasMomentum ? -(l2*cosalpha + sinalpha)/den4 : 0.);
      //
      j(4,0) = 0.;
      j(4,1) = 0.;
      j(4,2) = (cosalpha*sinbeta*l2 - cosbeta*l2*l3 + sinalpha*sinbeta*(l3*l3+1.))/den23;
      j(4,3) = (sinbeta*l3*(cosalpha-sinalpha*l2) + cosbeta*(l2*l2+1.))/den23;
      j(4,4) = (hasMomentum ? (cosalpha*sinbeta - cosbeta*l3 - sinalpha*sinbeta*l2)/den4 : 0.);
      //
      j(5,0) = 0.;
      j(5,1) = 0.;
      j(5,2) = -(cosalpha*cosbeta*l2 + cosbeta*sinalpha*l3*l3 + cosbeta*sinalpha + sinbeta*l2*l3)/den23;
      j(5,3) = (-cosalpha*cosbeta*l3 + cosbeta*sinalpha*l2*l3 + sinbeta*l2*l2 + sinbeta)/den23;
      j(5,4) = (hasMomentum ? (-cosalpha*cosbeta + cosbeta*sinalpha*l2 - sinbeta*l3)/den4 : 0.);
      //
      return j;
    }

    SMatrix56 Plane::Global6DToLocal5DJacobian(bool hasMomentum, const Vector_t& trackMomOrDir, const Vector_t& planeDir, const TrigCache& trigCache) const {
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      const double den23 = (cosalpha*(cosbeta*trackMomOrDir.Z() - sinbeta*trackMomOrDir.Y()) + sinalpha*trackMomOrDir.X())*(cosalpha*(cosbeta*trackMomOrDir.Z() - sinbeta*trackMomOrDir.Y()) + sinalpha*trackMomOrDir.X());
      const double den4 = sqrt(trackMomOrDir.X()*trackMomOrDir.X()+trackMomOrDir.Y()*trackMomOrDir.Y()+trackMomOrDir.Z()*trackMomOrDir.Z())*(trackMomOrDir.X()*trackMomOrDir.X()+trackMomOrDir.Y()*trackMomOrDir.Y()+trackMomOrDir.Z()*trackMomOrDir.Z());
      SMatrix56 j;
      //
      j(0,0) = cosalpha;
      j(0,1) = sinalpha*sinbeta ;
      j(0,2) = -sinalpha*cosbeta;
      j(0,3) = 0.;
      j(0,4) = 0.;
      j(0,5) = 0.;
      //
      j(1,0) = 0.;
      j(1,1) = cosbeta;
      j(1,2) = sinbeta;
      j(1,3) = 0.;
      j(1,4) = 0.;
      j(1,5) = 0.;
      //
      j(2,0) = 0.;
      j(2,1) = 0.;
      j(2,2) = 0.;
      j(2,3) = ((cosalpha*cosalpha + sinalpha*sinalpha)*(cosbeta*trackMomOrDir.Z() - sinbeta*trackMomOrDir.Y()))/den23;
      j(2,4) = (sinbeta*trackMomOrDir.X()*(cosalpha*cosalpha + sinalpha*sinalpha))/den23;
      j(2,5) = -(cosbeta*trackMomOrDir.X()*(cosalpha*cosalpha + sinalpha*sinalpha))/den23;
      //
      j(3,0) = 0.;
      j(3,1) = 0.;
      j(3,2) = 0.;
      j(3,3) = -(sinalpha*(cosbeta*trackMomOrDir.Y() + sinbeta*trackMomOrDir.Z()))/den23;
      j(3,4) = (cosalpha*trackMomOrDir.Z()*(cosbeta*cosbeta + sinbeta*sinbeta) + cosbeta*sinalpha*trackMomOrDir.X())/den23;
      j(3,5) = (-cosalpha*cosbeta*cosbeta*trackMomOrDir.Y() - cosalpha*sinbeta*sinbeta*trackMomOrDir.Y() + sinalpha*sinbeta*trackMomOrDir.X())/den23;
      //
      j(4,0) = 0.;
      j(4,1) = 0.;
      j(4,2) = 0.;
      j(4,3) = (hasMomentum ? -trackMomOrDir.X()/den4 : 0.);
      j(4,4) = (hasMomentum ? -trackMomOrDir.Y()/den4 : 0.);
      j(4,5) = (hasMomentum ? -trackMomOrDir.Z()/den4 : 0.);
      //
      return j;
    }

    Rotation_t Plane::Global3DToLocal3DRotation(const Vector_t& planeDir, const TrigCache& trigCache) const {
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      return {
	cosalpha /* xx */,  sinalpha * sinbeta /* xy */, -sinalpha * cosbeta /* xz */,
	0.0      /* yx */,  cosbeta            /* yy */,  sinbeta            /* yz */,
	sinalpha /* zx */, -cosalpha * sinbeta /* zy */,  cosalpha * cosbeta /* zz */
      };
    }

    Rotation_t Plane::Local3DToGlobal3DRotation(const Vector_t& planeDir, const TrigCache& trigCache) const {
      const double& sinalpha = trigCache.fSinA;
      const double& cosalpha = trigCache.fCosA;
      const double& sinbeta  = trigCache.fSinB;
      const double& cosbeta  = trigCache.fCosB;
      return {
	cosalpha            /* xx */, 0.      /* xy */,  sinalpha           /* xz */,
	sinalpha * sinbeta  /* yx */, cosbeta /* yy */, -cosalpha * sinbeta /* yz */,
	-sinalpha * cosbeta /* zx */, sinbeta /* zy */,  cosalpha * cosbeta /* zz */
      };
    }

  }
}
