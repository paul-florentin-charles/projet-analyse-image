#include "root.hpp"

void process(const char * imsname, const char * imdname)
{
  Mat ims = imread(imsname, 0);
  if (!ims.data) {
    cerr << "Could not open or find the image corresponding to " << imsname << "\n" << endl;
    exit(EXIT_FAILURE);
  }

  Mat imd;
  ims.copyTo(imd);

  int width = ims.cols, height = ims.rows;

  CvSVMParams parameters;
  parameters.svm_type = CvSVM::C_SVC;
  parameters.kernel_type = CvSVM::LINEAR;
  parameters.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

  // TODO : List of labels + corresponding surfaces for each label
  /*
  ** float labels[n_cells] = {a, b, a, c};
  ** Mat m_labels(n_cells, 1, CV_32FC1, labels);
  **
  ** float data[n_cells] = {surface_1, surface_2, surface_3, surface_4};
  ** Mat m_data(n_cells, 1, CV_32FC1, data);
  */

  // Un bon critère serait le rapport surface_noyau / surface_cellule, mais on ne peut l'avoir pour des images binaires...

  float labels[5] = {0.0, 0.0, 1.0, 0.0, 0.0};
  Mat m_labels(5, 1, CV_32FC1, labels);

  float data[5] = {81656.3, 67958.9, 65370.3, 46470.4, 48657.0};
  Mat m_data(5, 1, CV_32FC1, data);

  CvSVM model;
  // TODO : Train model with the previous lists
  model.train(m_data, m_labels, Mat(), Mat(), parameters);

  vector<unsigned int> surfaces(256, 0);
  for (int i = 0; i < ims.rows; i++) {
    for (int j = 0; j < ims.cols; j++) {
      if (ims.at<unsigned char>(i, j)) surfaces[ims.at<unsigned char>(i, j)]++;
    }
  }

  float response;
  vector<unsigned char> responses(256, 0);
  for (int k = 1; k < 256; k++) {
    Mat sample(1, 1, CV_32FC1, surfaces[k]);
    response = model.predict(sample);
    switch ((int) response) {
      case 0: // Monocytes
        responses[k] = 50;
        break;
      case 1: // Lymphocytes
        responses[k] = 100;
        break;
      case 2: // Polynucléaires neutrophiles
        responses[k] = 150;
        break;
      case 3: // Polynucléaires eosinophiles
        responses[k] = 200;
        break;
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      imd.at<unsigned char>(i, j) = responses[ims.at<unsigned char>(i, j)];
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
