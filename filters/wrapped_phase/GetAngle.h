// --------- March 2017, Ian L Jones ---------
//
//GetAngle
//    ->SetInput(DOUBLE VEC VOL, Phi);
//    ->GetOutput(DOUBLE VOL);
//

#ifndef __GetAngle_h
#define __GetAngle_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< typename TInputPhi, typename TOutputWrappedPhase>
class GetAngle : public ImageToImageFilter< TInputPhi,  TOutputWrappedPhase > // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef GetAngle             Self;
    typedef ImageToImageFilter< TInputPhi,  TOutputWrappedPhase > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GetAngle, ImageToImageFilter);

    // Input images
    void SetInput(const TInputPhi* phi);

    // Output
    TOutputWrappedPhase* GetOutput();

protected:
    GetAngle();
    ~GetAngle() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TInputPhi::ConstPointer GetInput();

    /**  Create the Output */
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    GetAngle(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "GetAngle.hxx"
#endif


#endif // __GetAngle_h
