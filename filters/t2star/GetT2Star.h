
// --------- March 2017, Ian L Jones ---------
//
//GetT2Star
//    ->SetInputA(DOUBLE VOL, A);
//    ->SetT2StarMax(DOUBLE VAL, T2STAR_MAX);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __GetT2Star_h
#define __GetT2Star_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputA, typename TOutputT2Star>
class GetT2Star : public ImageToImageFilter< TInputA,  TOutputT2Star > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetT2Star             Self;
    typedef ImageToImageFilter< TInputA,  TOutputT2Star > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetT2Star, ImageToImageFilter);

    // Input images
    void SetInputA(const TInputA* a);

    itkSetGetDecoratedInputMacro(TeVal1, double );
    itkSetGetDecoratedInputMacro(TeVal2, double );
    itkSetGetDecoratedInputMacro(TeVal3, double );
    itkSetGetDecoratedInputMacro(T2StarMax, double );

    // Output
    TOutputT2Star* GetOutput();

protected:
    GetT2Star();
    ~GetT2Star() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInputA::ConstPointer GetInputA();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    GetT2Star(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "GetT2Star.hxx"
#endif


#endif // __GetT2Star_h
