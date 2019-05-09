
// --------- March 2017, Ian L Jones ---------
//    
//GetDeltaB0
//    ->SetInputPhi(DOUBLE VOL, unwrappedPhase);
//    ->SetInputMask(DOUBLE VOL, maskNorm);
//    ->SetDeltaFMax(DOUBLE VAL, DELTA_F_MAX);
//    ->SetGammaH(DOUBLE VAL, GAMMAH_H);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL );
//

#ifndef __GetDeltaB0_h
#define __GetDeltaB0_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TPhi, typename TMask, typename TOut>
class GetDeltaB0 : public ImageToImageFilter< TPhi, TOut> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetDeltaB0             Self;
    typedef ImageToImageFilter< TPhi,  TOut> Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetDeltaB0, ImageToImageFilter);

    // Input images
    void SetInputPhi(const TPhi* wrappedPhase);
    void SetInputMask(const TMask* mask);

    itkSetGetDecoratedInputMacro(DeltaFMax,  double);// max delta freq [Hz]
    itkSetGetDecoratedInputMacro(GammaH,  double);//1H gyromagnetic constant [Hz/T]
    itkSetGetDecoratedInputMacro(TeVal1, double );
    itkSetGetDecoratedInputMacro(TeVal2, double );
    itkSetGetDecoratedInputMacro(TeVal3, double );

    // Output
    TOut* GetOutput();

protected:
    GetDeltaB0();
    ~GetDeltaB0() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TPhi::ConstPointer GetInputPhi();
    typename TMask::ConstPointer GetInputMask();


    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    GetDeltaB0(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "GetDeltaB0.hxx"
#endif


#endif // __GetDeltaB0_h
