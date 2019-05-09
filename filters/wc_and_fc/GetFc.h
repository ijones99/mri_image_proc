// --------- March 2017, Ian L Jones ---------
//
//GetFc
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetInputPc(DOUBLE VOL, Pc);
//    ->SetInputI1(DOUBLE VEC VOL, I1);
//    ->SetInputA(DOUBLE VOL, A);
//    ->SetWcScale(DOUBLE VAL, FC_SCALE);
//    ->SetSMax(DOUBLE VAL, FC_S_MAX);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetFc_h
#define __GetFc_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TI0, typename TPc, typename TI1,typename TA, typename TOut>
class GetFc : public ImageToImageFilter< TI0, TOut> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetFc             Self;
    typedef ImageToImageFilter< TI0,  TOut> Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetFc, ImageToImageFilter);

    // Input images
    void SetInputI0(const TI0* i0);
    void SetInputPc(const TPc* pc);
    void SetInputI1(const TI1* i1);
    void SetInputA(const TA* a);

        itkSetGetDecoratedInputMacro(FcScale,  double);
    itkSetGetDecoratedInputMacro(SMax,  double);

    // Output
    TOut* GetOutput();

protected:
    GetFc();
    ~GetFc() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TI0::ConstPointer GetInputI0();
    typename TPc::ConstPointer GetInputPc();
    typename TI1::ConstPointer GetInputI1();
    typename TA::ConstPointer GetInputA();

    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    GetFc(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "GetFc.hxx"
#endif


#endif // __GetFc_h
