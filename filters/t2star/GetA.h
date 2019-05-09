// --------- March 2017, Ian L Jones ---------
//
//GetA
//    ->SetInputI2(DOUBLE VEC VOL, I2);
//    ->SetInputI0(DOUBLE VEC VOL, I0);
//    ->SetTeVal1(DOUBLE VAL, teVal1);
//    ->SetTeVal2(DOUBLE VAL, teVal2);
//    ->SetTeVal3(DOUBLE VAL, teVal3);
//    ->GetOutput(DOUBLE VOL );
//


#ifndef __GetA_h
#define __GetA_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputI2, typename TInputI0, typename TOutput>
class GetA : public ImageToImageFilter< TInputI2,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetA             Self;
    typedef ImageToImageFilter< TInputI2,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetA, ImageToImageFilter);

    // Input images
    void SetInputI2(const TInputI2* imageI2);
    void SetInputI0(const TInputI0* imageI0);

    itkSetGetDecoratedInputMacro(TeVal1, double );
    itkSetGetDecoratedInputMacro(TeVal2, double );
    itkSetGetDecoratedInputMacro(TeVal3, double );

    // Output
    TOutput* GetOutput();

protected:
    GetA();
    ~GetA() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInputI2::ConstPointer GetInputI2();
    typename TInputI0::ConstPointer GetInputI0();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    GetA(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "GetA.hxx"
#endif


#endif // __GetA_h
