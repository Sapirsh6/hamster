#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
using namespace std;
using namespace HamsterAPI;

int main() {
	try {
		Hamster* hamster = new HamsterAPI::Hamster(1);

		cv::namedWindow("OccupancyGrid-view");
		while (hamster->isConnected()) {
			try {

				OccupancyGrid ogrid = hamster->getSLAMMap();
				int width = ogrid.getWidth();
				int height = ogrid.getHeight();
				unsigned char pixel;
				//CvMat* M = cvCreateMat(width, height, CV_8UC1);

				//TODO: VEC3d map.at<cv::ve3d>.at((i,j)[0] = 0
				cv::Mat m = cv::Mat(width, height,CV_8UC1);

				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++) {
						if (ogrid.getCell(i, j) == CELL_FREE)
							pixel = 255;
						else if (ogrid.getCell(i, j) == CELL_OCCUPIED)
							pixel = 0;
						else
							pixel = 128;
						//cvmSet(M, i, j, pixel);
						m.at<unsigned char>(i,j) = pixel;
					}

				//nipuah
				double reso = ogrid.getResolution();
				reso = (int)reso%10;
				cv::Mat mCopy = cv::Mat(width+reso*2, height+reso*2,CV_8UC1);
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						if (m.at<unsigned char>(i,j) == CELL_OCCUPIED)
						{
							pixel = 0;
							for(int m = reso+i; m<=reso; m++)
							{
								for(int n = reso+j; n<=reso; n++)
								{
									mCopy.at<unsigned char>(i,j) = pixel;
								}
							}
							//Copy the MatrixCopy to the orignal one
							for (int i = reso; i < height; i++)
							{
								for (int j = reso; j < width; j++)
								{
									m.at<unsigned char>(i,j) = mCopy.at<unsigned char>(i,j);
								}
							}
						}
					}
				}

				cv::imshow("OccupancyGrid-view",m);
				cv::waitKey(1);

			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}
