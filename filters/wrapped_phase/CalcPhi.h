
// --------- March 2017, Ian L Jones ---------
//
//CalcPhi
//    ->SetInputI0(DOUBLE VEC VOL, I0)
//    ->SetInputI2(DOUBLE VEC VOL, I2);
//    ->GetOutput(DOUBLE VEC VOL)
//
//



#ifndef __CalcPhi_h
#define __CalcPhi_h

#include "itkImageToImageFilter.h"

namespace itk
{
template< class TImage>
class CalcPhi : public ImageToImageFilter< TImage, TImage> // This doesn't actually matter, as we will be overriding the output image type in MakeOutput()
{
public:
    /** Standard class typedefs. */
    typedef CalcPhi             Self;
    typedef ImageToImageFilter< TImage,  TImage > Superclass;
    typedef SmartPointer< Self >        Pointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(CalcPhi, ImageToImageFilter);

    // Input images
    void SetInputI0(const TImage* I0);
    void SetInputI2(const TImage* I2);

    // Output
    TImage* GetOutput();

protected:
    CalcPhi();
    ~CalcPhi() {}

    /** Does the real work. */
    virtual void GenerateData();

    typename TImage::ConstPointer GetInputI0();
    typename TImage::ConstPointer GetInputI2();

    // Create the Output
    DataObject::Pointer MakeOutput(unsigned int idx);

private:
    CalcPhi(const Self &); //purposely not implemented
    void operator=(const Self &);  //purposely not implemented

};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATI0N
#include "CalcPhi.hxx"
#endif


#endif // __CalcPhi_h
