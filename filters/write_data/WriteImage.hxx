#ifndef __WriteImage_txx
#define __WriteImage_txx

#include "WriteImage.h"

#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

template< class TImage>
WriteImage<TImage>::WriteImage()
{

    this->SetNumberOfRequiredInputs(1);

}

template< class TImage>
void WriteImage<TImage>::SetInput(const TImage* image)
{
    this->SetNthInput(0, const_cast<TImage*>(image));
}


template< class TImage>
typename TImage::ConstPointer WriteImage<TImage>::GetInput()
{
    return static_cast< const TImage * >
           ( this->ProcessObject::GetInput(0) );
}


template< class TImage>
void WriteImage<TImage>::GenerateData()
{


    std::string outFileName = this->GetFileName();

    typedef itk::ImageFileWriter< TImage > WriterType;

    typename WriterType::Pointer writer = WriterType::New();

    writer->SetFileName(outFileName);
    writer->UseCompressionOn();
    writer->SetInput(this->GetInput());

    std::cout << "Writing: " << outFileName << std::endl;
    try
    {
        std::cout <<"" << std::endl;
        writer->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
    }

}




}// end namespace


#endif
