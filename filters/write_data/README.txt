write_data/

SUMMARY:
	WriteImage: Filter used to write a volume object to an *mha file.

USAGE:
	typedef itk::WriteImage<ImageTypeDoubleVol>  FilterWriteImage;
	rwFileName = "maskNorm.mha";
    	FilterWriteImage::Pointer filterWriteImage = FilterWriteImage::New();
    	filterWriteImage->SetInput(maskNorm);
    	filterWriteImage->SetFileName(FileName);
    	filterWriteImage->Update();
