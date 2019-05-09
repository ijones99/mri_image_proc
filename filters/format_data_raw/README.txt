SUMMARY:
	Used to replicate the way in which the Matlab script Dixon3D.m makes all computations on data. This filter exchanges the x and y axes and reverses the z axis.

USAGE:

	// Example usage
	typedef itk::Transpose<ImageTypeDoubleVol>  FilterTranspose;
	FilterTranspose::Pointer filterTranspose = FilterTranspose::New();
	filterTranspose->SetInput(imageS1magRead );
	filterTranspose->Update();
	typename ImageTypeDoubleVol::Pointer imageS1mag = filterTranspose->GetOutput();

INPUT: 
	3d volume

OUTPUT: 
	3d volume




