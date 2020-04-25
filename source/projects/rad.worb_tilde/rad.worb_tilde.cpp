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
  MIN_RELATED{"rad.fpu, rad.elca"};

  inlet<> freq_in{this, "(number) base frequency"};
  inlet<> period_in{this, "(number) period"};
  outlet<> osc_out{this, "(signal) oscillator output", "signal"};

  attribute<number> frequency{
      this, "frequency", 1.0, description{"base update frequency in Hz"},
      setter{MIN_FUNCTION{
        theWorb.setRate(args[0], samplerate());
        return args;
      }}
  };

  attribute<number> period{
      this, "period", 1.0, description{"period to search for (integer)"},
      setter{MIN_FUNCTION{
          theWorb.setPeriod(static_cast<int>(args[0]));
          return args;
      }}
  };

  attribute<number> gamma{
      this, "gamma", 1.0, description{"gamma parameter"},
      setter{MIN_FUNCTION{
          theWorb.setGamma(args[0]);
          return args;
      }}
  };

  message<> m_number{this, "number", "", MIN_FUNCTION {
    if (inlet == 0) {
       theWorb.setRate(args[0]);
    }
    else if (inlet == 1) {
      theWorb.setPeriod(static_cast<int>(args[0]));
    }
    else {
      theWorb.setGamma(args[0]);
    }
    return {};
  }};

  sample operator()() { return theWorb.next(); }
};

MIN_EXTERNAL(worb);
