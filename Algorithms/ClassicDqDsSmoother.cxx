#ifndef CLASSICSDQDSSMOOTHER_CXX
#define CLASSICSDQDSSMOOTHER_CXX

#include "ClassicDqDsCalculator.h"


namespace cosmictag {

  static ClassicDqDsCalculatorFactory __global_ClassicDqDsCalculatorFactory__;

  ClassicDqDsCalculator::ClassicDqDsCalculator(const std::string name)
    : BaseHitOrdererAlgo(name)
  {}

  void ClassicDqDsCalculator::_Configure_(const Config_t &pset)
  {
    _slider_window = pset.get<int>("GlobalQE");
  }
  
  int ClassicDqDsCalculator::CalculateDqDs(SimpleCluster& cluster) const
  {

    //int                    & _start_index      = cluster._start_index;
    std::vector<SimpleHit> & _s_hit_v          = cluster._s_hit_v;
    std::vector<double>    & _dqds_v           = cluster._dqds_v;
    std::vector<double>    & _dqds_slider      = cluster._dqds_slider;
    //bool                   & _hits_ordered     = cluster._hits_ordered;
    //std::vector<double>    & _ds_v        = cluster._ds_v;

     if (_dqds_v.size() != _s_hit_v.size()) {
      CT_ERROR() << " dqds size is different than hit vector size" << std::endl;
      throw HitCosmicTagException();
    }

    if (_dqds_v.size() < _slider_window * 2) {
      CT_NORMAL() <<  " not enough hits" << std::endl;
      return 0;
    }

    //size_t window = _slider_window;

    if (_slider_window % 2 != 0) {
      CT_ERROR() << ": _slider_window has to be even." << std::endl; 
      throw HitCosmicTagException();
    }

    _dqds_slider.clear();
    _dqds_slider.reserve(_dqds_v.size());


    for(const auto& window : get_windows(_dqds_v, _slider_window)) {

      double median_dqds = get_smooth_trunc_median(window);
      _dqds_slider.push_back(median_dqds);
      //if (_debug) std::cout << "dqds_slider value " << median_dqds << std::endl;


    }

    return _s_hit_v.size();
  }
}
#endif