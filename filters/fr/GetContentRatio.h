
// --------- March 2017, Ian L Jones ---------
//
//GetContentRatio
//    ->SetInput1(DOUBLE VOL, fcMask);
//    ->SetInput2(DOUBLE VOL, wcMask);
//    ->GetOutput(DOUBLE VOL);
//


#ifndef __GetContentRatio_h
#define __GetContentRatio_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput1, typename TInput2, typename TOutput>
class GetContentRatio : public ImageToImageFilter< TInput1,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetContentRatio             Self;
    typedef ImageToImageFilter< TInput1,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetContentRatio, ImageToImageFilter);

    // Input images
    void SetInput1(const TInput1* image1);
    void SetInput2(const TInput2* image2);

    // Output
    TOutput* GetOutput();

protected:
    GetContentRatio();
    ~GetContentRatio() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput1::ConstPointer GetInput1();
    typename TInput2::ConstPointer GetInput2();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    GetContentRatio(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "GetContentRatio.hxx"
#endif


#endif // __GetContentRatio_h
