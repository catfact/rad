#pragma once

#include "Interpolator.h"

class Osc {

protected:
    // pointer current interpolator
    Interpolator *interp;
    // array of all interpolators
    Interpolator *interps[Interpolator::NumInterpTypes];

    Hold interpHold;
    Ramp interpRamp;
    Cube interpCube;
    Sine interpSine;
    Slew interpSlew;
    Smooth interpSmooth;
    Decay interpDecay;

    //-- oscillator state
    double phase;
    double inc;
    float rate;
    float sr;
    float interpSpeed;

    //-- arbitrary numerical index of this osc
    int id;

public:
    Osc() {
        interpSpeed = 1.f;
        interps[Interpolator::Hold] = &interpHold;
        interps[Interpolator::Ramp] = &interpRamp;
        interps[Interpolator::Cube] = &interpCube;
        interps[Interpolator::Sine] = &interpSine;
        interps[Interpolator::Slew] = &interpSlew;
        interps[Interpolator::Smooth] = &interpSmooth;
        interps[Interpolator::Decay] = &interpDecay;
    }

    void setInterpType(Interpolator::Type t) {
        Interpolator *newInterp;
        switch (t) {
            case Interpolator::Hold:
                newInterp = &interpHold;
                break;
            case Interpolator::Ramp:
                newInterp = &interpRamp;
                break;
            case Interpolator::Cube:
                newInterp = &interpCube;
                break;
            case Interpolator::Sine:
                newInterp = &interpSine;
                break;
            case Interpolator::Slew:
                newInterp = &interpSlew;
                break;
            case Interpolator::Smooth:
                newInterp = &interpSmooth;
                break;
            case Interpolator::Decay:
                newInterp = &interpDecay;
                break;
            default:;;
        }
        newInterp->copyState(interp);
        interp = newInterp;
    }

    float getNextSample(int numFrames) {
        for (int i = 0; i < numFrames; ++i) {
            phase += inc;
            while (phase > 1.0) {
                interp->push(this->update());
                phase -= 1.0;
            }
            buf[i] += interp->update();
        }
    }

    void setRate(float rate_hz) {
        rate = rate_hz;
        inc = rate / sr;
        interp->setRate(rate_hz * interpSpeed, sr);
    }

    void setRate(float rate_hz, float sr_hz) {
        sr = sr_hz;
        setRate(rate_hz);
    }

    void setInterpSpeed(float r) {
        interpSpeed = r;
        interp->setRate(rate * interpSpeed, sr);
    }

 private:
    virtual void update() = 0;

};
		 
