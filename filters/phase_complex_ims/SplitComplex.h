
// --------- March 2017, Ian L Jones ---------
//   
//SplitComplex
//    ->SetInput(DOUBLE VOL VEC)
//    ->GetOutput1(DOUBLE VOL);
//    ->GetOutput2(DOUBLE VOL);
//
//


#ifndef __SplitComplex_h
#define __SplitComplex_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput, typename TOutput1, typename TOutput2>
class SplitComplex : public ImageToImageFilter< TInput,  TOutput2 > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef SplitComplex             Self;
    typedef ImageToImageFilter< TInput,  TOutput2 > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(SplitComplex, ImageToImageFilter);

    // Input images
    void SetInput(const TInput* imageComplex);

    // Output
    TOutput1* GetOutput1();
    TOutput2* GetOutput2();

protected:
    SplitComplex();
    ~SplitComplex() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput::ConstPointer GetInput();


    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    SplitComplex(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "SplitComplex.hxx"
#endif


#endif // __SplitComplex_h
