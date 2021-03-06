/**
 * \file HouseholderMixture.cpp
 */

#include <ATK/Delay/HouseholderMixture.hxx>

#include <complex>

namespace ATK
{
  template class FeedbackDelayNetworkFilter<HouseholderMixture<float, 2>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<double, 2>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<std::complex<float>, 2>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<std::complex<double>, 2>>;

  template class FeedbackDelayNetworkFilter<HouseholderMixture<float, 4>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<double, 4>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<std::complex<float>, 4>>;
  template class FeedbackDelayNetworkFilter<HouseholderMixture<std::complex<double>, 4>>;
}
