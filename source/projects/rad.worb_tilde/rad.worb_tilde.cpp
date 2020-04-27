//
// Created by e on 10/29/19.
//

#include "c74_min.h"
#include "rad-lib/worb.h"

using namespace c74::min;

class worb : public object<worb>, public sample_operator<0, 1> {
private:
    rad::Worb theWorb;
    
public:
    MIN_DESCRIPTION{"locked-orbit chaotic map oscillator."};
    MIN_TAGS{"audio, chaotic, oscillators"};
    MIN_AUTHOR{"catfact + dr_rad"};
    MIN_RELATED{""};
    
    inlet<> in1 {this, "(number) base frequency"};
    inlet<> in2 {this, "(number) period (in [1, ?])"};
    inlet<> in3 {this, "(number) gamma (smoothing parameter)"};
    inlet<> in4 {this, "(number) beta (base logistic parameter)"};
    inlet<> in5 {this, "(number) noise (added to logistic param)"};
    inlet<> in6 {this, "(number) interpolation type (in [0-3])"};
    outlet<> out1 {this, "(signal) oscillator output", "signal"};
    
    message<> msg_number{this, "number", "", MIN_FUNCTION {
        if (inlet == 0) {
            theWorb.setRate(args[0]);
        }
        else if (inlet == 1) {
            theWorb.setPeriod(std::max(static_cast<int>(args[0]), 1));
        }
        else if (inlet == 2) {
            theWorb.setGamma(args[0]);
        }
        else if (inlet == 3) {
            theWorb.setBase(args[0]);
        }
        else if (inlet == 4) {
            theWorb.setNoise(args[0]);
        }
        else if (inlet == 5) {
            theWorb.setInterpType(static_cast<rad::Interpolator::Type>(std::min(3, std::max(0, static_cast<int>(args[0])))));
        }

        return {};
    }};
    
    message<> msg_reset { this, "reset", "reset map history with initial value", MIN_FUNCTION {
        cout << "resetting the Worb with value: " << args[0] << endl;
        theWorb.reset(args[0]);
        return {};
    }};
        
    message<> msg_randomize { this, "randomize", "randomize map history", MIN_FUNCTION {
        cout << "randomizing the Worb" << endl;
        theWorb.randomize();
        return {};
    }};
    
        
    worb(const atoms& args = {}) {
        if (args.size() > 0) {
            theWorb.reset(args[0]);
        }
    }

    sample operator()() {
        return std::min(1.f, std::max(-1.f, theWorb.processSample()));
    }
        
    message<> dspsetup { this, "dspsetup",
        MIN_FUNCTION {
        theWorb.setSampleRate(args[0]);
            return {};
        }
    };

};

MIN_EXTERNAL(worb);
