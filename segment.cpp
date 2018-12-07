#include "root.hpp"

void train(CvSVM& model, CvSVMParams& parameters, Mat& ims) {
  int width = ims.cols, height = ims.rows;
  int n_points = width * height;

  float labels[n_points];
  Mat m_labels(n_points, 1, CV_32FC1, labels);

  float data[n_points][2];
  Mat m_data(n_points, 2, CV_32FC2, data);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      data[j + i * width][0] = i, data[j + i * width][1] = j;
      if (ims.at<unsigned char>(i, j) == 128) // Change for label of nucleus
        labels[j + i * width] = 2.0;
      else if (ims.at<unsigned char>(i, j) == 255) // Change for label of cytoplasm
        labels[j + i * width] = 1.0;
      else
        labels[j + i * width] = 0.0;
    }
  }

  model.train(m_data, m_labels, Mat(), Mat(), parameters);
}

void process(const char * imsname, const char * imdname)
{
  Mat ims = imread(imsname, 0);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  Mat imd;
  ims.copyTo(imd);

  CvSVMParams parameters;
  parameters.svm_type = CvSVM::C_SVC;
  parameters.kernel_type = CvSVM::LINEAR;
  parameters.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

  CvSVM model;
  train(model, parameters, ims);

  int width = ims.cols, height = ims.rows;

  float response;
  float point[2];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      point[0] = i, point[1] = j;
      Mat sample(1, 1, CV_32FC2, point);
      response = model.predict(sample);
      switch ((int) response) {
        case 0: // Fond
          imd.at<unsigned char>(i, j) = 100;
          break;
        case 1: // Cytoplasme
          imd.at<unsigned char>(i, j) = 160;
          break;
        case 2: // Noyau
          imd.at<unsigned char>(i, j) = 220;
          break;
      }
    }
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
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
