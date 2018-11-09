// A useful filter, behaves like a smoothly parameterized smooth Heaviside
// function.
// 
// Inputs:
//   x  input scalar (-inf, inf)
//   t  control steepness of step function: --> 0 more linear, --> inf more like
//     Heaviside function (piecewise constant function x<0--> -1 , x>0 --> 1)
//     **SEE BUG BELOW**
// Returns scalar value 
//
// Known issue: this is actually offset so that x<0.5-->-1 and x>0.5-->1. This
// will be fixed in later versions but no earlier than Winter 2019.
float smooth_heaviside( float x, float t)
{
  return (1./(1.+exp(-2.*t*(2.*x-1.)))-1./2.)/(1./(1.+exp(-2.*t*1.))-1./2.);
}

