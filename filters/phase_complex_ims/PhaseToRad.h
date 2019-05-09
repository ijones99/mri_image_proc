//
// --------- March 2017, Ian L Jones ---------
//    
//PhaseToRad
//    SetInput(DOUBLE VOL); 
//    SetMaxPhaseVal(DOUBLE VAL);
//

#ifndef __itkPhaseToRad_h
#define __itkPhaseToRad_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class PhaseToRad:public ImageToImageFilter< TImage, TImage >
{
public:

  /** Standard class typedefs. */
  typedef PhaseToRad             Self;
  typedef ImageToImageFilter< TImage, TImage > Superclass;
  typedef SmartPointer< Self >        Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhaseToRad, ImageToImageFilter);

  itkSetGetDecoratedInputMacro(MaxPhaseVal, double );

  itkSetMacro( Variable, float );
  itkGetMacro( Variable, float);

protected:
  PhaseToRad(){}
  ~PhaseToRad(){}

  /** Does the real work. */
  virtual void GenerateData();

  float m_Variable;

private:
  PhaseToRad(const Self &); //purposely not implemented
  void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "PhaseToRad.hxx"
#endif


#endif // __itkPhaseToRad_h
