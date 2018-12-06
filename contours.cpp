#include "root.hpp"

RNG rng(12345);

void process(const char * imsname, const char * imdname)
{
  Mat ims = imread(imsname, 0);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours(ims, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  Mat imd = Mat::zeros(ims.size(), CV_8UC3);
  for (size_t i = 0; i < contours.size(); i++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
    drawContours(imd, contours, i, color, 2, 8, hierarchy, 0, Point());
  }

  imwrite(imdname, imd);
}

void usage(const char *s)
{
  cerr << "Usage: " << s << " input_img_path output_img_name" << endl;
  exit(EXIT_FAILURE);
}

#define param 2
int main(int argc, char * argv[])
{
  if(argc != (param + 1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
