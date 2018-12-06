#include "root.hpp"

void process(unsigned int K, const char* imsname, const char* imdname)
{
  Mat ims = imread(imsname);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  int width = ims.cols, height = ims.rows;

  Mat imd;
  ims.copyTo(imd);

  vector<Point2f> points(width * height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      points[j + i * width] = Point2f((ims.at<Vec3b>(i, j)[0] + ims.at<Vec3b>(i, j)[1]) / 2, (ims.at<Vec3b>(i, j)[1] + ims.at<Vec3b>(i, j)[2]) / 2);
      // TODO : better conversion from Vec3b to Point2f
    }
  }

  vector<int> labels(width * height);
  kmeans(points, K, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_PP_CENTERS, noArray());

  int label, color;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      label = labels[j + i * width];
      color = (float) label / (K - 1) * 255;
      imd.at<Vec3b>(i, j) = Vec3b(color, color, color);
    }
  }

  imwrite(imdname, imd);
}

void usage(const char * s)
{
  cerr << "Usage: " << s << " n_clusters input_img_path output_img_name" << endl;
  exit(EXIT_FAILURE);
}

#define param 3
int main(int argc, char* argv[])
{
  if(argc != (param + 1)) usage(argv[0]);

  process(atoi(argv[1]), argv[2], argv[3]);
  return EXIT_SUCCESS;
}
