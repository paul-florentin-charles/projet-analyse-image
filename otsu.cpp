#include "root.hpp"

void process(const char * imsname, const char * imdname)
{
  Mat ims = imread(imsname, 0);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  Mat otsu_ims;
  ims.copyTo(otsu_ims);

  unsigned int thresh = threshold(ims, otsu_ims, 0, 255, THRESH_BINARY + THRESH_OTSU);
  for (int i = 0; i < ims.rows; i++) {
    for (int j = 0; j < ims.cols; j++) {
      if (ims.at<unsigned char>(i,j) <= thresh)
        otsu_ims.at<unsigned char>(i,j) = 0;
      else
        otsu_ims.at<unsigned char>(i,j) = 255;
    }
  }

  imwrite(imdname, otsu_ims);
}

void usage(const char *s)
{
  cerr << "Usage: " << s << " input_img_path output_img_name" << endl;
  exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char * argv[])
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
