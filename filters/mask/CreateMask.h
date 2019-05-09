// --------- March 2017, Ian L Jones ---------
//
//CreateMask
//    ->SetInput(DOUBLE VEC VOL, I0); 
//    ->SetMaskThreshold(DOUBLE VAL);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __CreateMask_h
#define __CreateMask_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInput, typename TOutput>
class CreateMask : public ImageToImageFilter< TInput,  TOutput > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef CreateMask             Self;
    typedef ImageToImageFilter< TInput,  TOutput > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(CreateMask, ImageToImageFilter);

    // Input images
    void SetInput(const TInput* imageI0);

    itkSetGetDecoratedInputMacro(MaskThreshold, double );

    // Output
    TOutput* GetOutput();

protected:
    CreateMask();
    ~CreateMask() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInput
    ::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int  idx);

private:
    CreateMask(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "CreateMask.hxx"
#endif


#endif // __CreateMask_h
