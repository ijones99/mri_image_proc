
// --------- March 2017, Ian L Jones ---------
//
//MagToComplex
//    ->SetInputImage1(DOUBLE VOL imageS1mag);
//    ->SetInputImage2(DOUBLE VOL imageS1PhaseRad);
//    ->GetOutput(DOUBLE VEC VOL);
//

#ifndef __MagToComplex_h
#define __MagToComplex_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputImage1, typename TInputImage2, typename TOutputImage1>
class MagToComplex : public ImageToImageFilter< TInputImage1,  TOutputImage1 > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef MagToComplex             Self;
    typedef ImageToImageFilter< TInputImage1,  TOutputImage1 > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(MagToComplex, ImageToImageFilter);

    // Input images
    void SetInputImage1(const TInputImage1* image1);
    void SetInputImage2(const TInputImage2* image2);

    // Output
    TOutputImage1* GetOutput();

protected:
    MagToComplex();
    ~MagToComplex() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInputImage1::ConstPointer GetInput1();
    typename TInputImage2::ConstPointer GetInput2();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    MagToComplex(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "MagToComplex.hxx"
#endif


#endif // __MagToComplex_h
