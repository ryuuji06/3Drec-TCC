/** OpenMVG TEST 15
 * 
 **/


/* ================================
 *  EXAMPLES
 * ================================
 * 
 * 
 * ================================
 *  COMMENTS
 * ================================
 * 
 * 
 * 
 */



#include "openMVG/image/image_io.hpp"

// FEATURE DETECTION AND DESCRIPTION
#include "openMVG/features/feature.hpp"
#include "openMVG/features/image_describer_akaze_io.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"

//#include "openMVG/cameras/Camera_Common.hpp"
//#include "openMVG/cameras/cameras.hpp"
//#include "openMVG/sfm/sfm_data.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <sys/time.h>

using namespace openMVG::image;
using namespace openMVG::features;
using namespace std;


// ==========================================================
// ENUMERATE to choose options
// ==========================================================
EDESCRIBER_PRESET stringToEnum(const uint8_t i)
{
  EDESCRIBER_PRESET preset;
  if (i==0)
    preset = NORMAL_PRESET;
  else
  if (i==1)
    preset = HIGH_PRESET;
  else
  if (i==2)
    preset = ULTRA_PRESET;
  else
    preset = EDESCRIBER_PRESET(-1);
  return preset;
}



int main(int argc, char **argv)
{
	// =================================================================
	//   ( 0 )   S E T   O P T I O N S
	// =================================================================
	
	//double focal = -1.0;
	//double distortion1=0.0, distortion2=0.0, distortion3=0.0;
	
	string sImageFilename = "kermit.jpg";
	string sView_filename = "edited.jpg";
	uint8_t iDescriberType = 0;
	uint8_t iDescPreset = 0;

	
	
	
	// =================================================================
	//   ( 1 )  SET INTRINSICS AND VIEWS (images and focal input)
	// =================================================================
	
	// TIMERS
	struct timeval start, end;
	long mtime, seconds, useconds;
	
	gettimeofday(&start,NULL);
	
	
	// send to embedded:
	// (1) describer type
	// (2) preset
	// (3) image size (maybe)
	// (4) camera intrinsics
	
	

	// =================================================================
	//   ( 2 )  FEATURE DETECTION AND DESCRIPTION
	// =================================================================

	
	// ==========================================================
	// (2.1) INITIALIZE DESCRIBER
	// ==========================================================
	
	unique_ptr<Image_describer> image_describer;
	
	switch (iDescriberType)
	{
		case 0:
			image_describer.reset(new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
		break;
		case 1:
			image_describer = AKAZE_Image_describer::create(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF));
		break;
	}
	
	image_describer->Set_configuration_preset(stringToEnum(iDescPreset));
    
    
    // ==========================================================
	// (2.3) UNDISTORT, SAVE IMAGE AND DETECT FEATURES
	// ==========================================================
	
	Image<RGBColor> imageRGB, undistRGB;
	Image<unsigned char> imageGray;
	
	// FOR EACH IMAGE:
	// (a) read with openMVG
	// (b) undistort
	// (c) detect features
	// (d) save image (to be read with openCV, be compressed and be sent)
	
	ReadImage(sImageFilename.c_str(), &imageRGB);
	
	// undistort
	/*
	shared_ptr<cameras::IntrinsicBase> cam;
	cam = make_shared<cameras::Pinhole_Intrinsic_Radial_K3>
		(view->ui_width, view->ui_height, p[0], p[1], p[2], distortion1, distortion2, distortion3);
	UndistortImage(imageRGB, cam.get(), undistRGB, BLACK);
	*/
	
	WriteImage(sView_filename.c_str(), undistRGB);
	// testar MVE sem thumbnail, ver se muda o resultado
	
	ConvertPixelType(undistRGB, &imageGray);
	
	auto regions = image_describer->Describe(imageGray);
	
	image_describer->Save(regions.get(), "image.feat","image.desc");
	
	gettimeofday(&end,NULL);
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = (1000000*seconds + useconds)/1000;
	
	// TEST PRINTS
	cout << "Regions: " << regions->RegionCount()
		 << "\tTime: " << mtime << " ms" << endl;
	
	return EXIT_SUCCESS;
    
}


