
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp> 
#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/videoio.hpp>
#include <iostream>
#include <vector>
#include <PSEyeVideoCapture.h>



using namespace cv;
using namespace std;

vector<Mat> preprocesamiento(Mat entrada);

int main(int argc, char** argv)
{
	//open the video file for reading
	//VideoCapture cap(0) ;
	PSEyeVideoCapture cap(0);
	

	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	//cap.read(cameraFrame);

	//Defining window names
	String windowNameOriginalVideo = "Original Video";
	String windowNameBrightnessHigh50 = "Brightness Increased by 50";
	String windowNameContrastHigh2 = "Contrast Increased by 2";

	//Create and open windows for above window names
	namedWindow(windowNameOriginalVideo, WINDOW_NORMAL);
	//namedWindow(windowNameBrightnessHigh50, WINDOW_NORMAL);
	//namedWindow(windowNameContrastHigh2, WINDOW_NORMAL);



	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video 

										 //Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}

		vector<Mat> salida0 = preprocesamiento(frame);
		namedWindow("circles0", 1);
		imshow("circles0", salida0[0]);
		namedWindow("R-c", 1);
		imshow("R-c", salida0[1]);
		//namedWindow("Rc", WINDOW_NORMAL);
		//imshow("Rc", salida0[2]);
		//namedWindow("cc", WINDOW_NORMAL);
		//imshow("cc", salida0[3]);

		imshow(windowNameOriginalVideo, frame);


		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stoppig the video" << endl;
			break;
		}
	}

	return 0;
}

vector<Mat> preprocesamiento(Mat entrada)
{
	vector<Mat> salida;
	RNG rng(12345);
	Mat canny_output;
	Canny(entrada, canny_output, 100, 100 * 2); // Detectar los bordes con un umbral min = 100 y max = 200

	vector<vector<Point> > v_contornos;
	vector<Vec4i> hierarchy;
	//// Buscar los contornos de la imagen, se almacenan en contours
	findContours(canny_output, v_contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Mostrar la Imagen modificada por la funcion findContours
	//imshow("Modificada", canny_output);
	Mat m_bordes = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i< v_contornos.size(); i++)
	{
		Scalar color = CV_RGB(50, 50, 0);
		drawContours(m_bordes, v_contornos, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	vector<Vec3f> circles;
	vector<Point> v_circ;
	HoughCircles(canny_output, circles, HOUGH_GRADIENT, 1, 10, 100, 30, 1, 35 // change the last two parameters
																			  // (min_radius & max_radius) to detect larger circles
	);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point n_c;
		n_c.x = circles[i][0];
		n_c.y = circles[i][1];
		v_circ.push_back(n_c);

		cout << circles[i][0] << " " << circles[i][1] << " " << circles[i][2] << " " << endl;
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// draw the circle center
		circle(m_bordes, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// draw the circle outline
		//circle(m_bordes, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
	salida.push_back(m_bordes);

	/// Find the rotated rectangles and ellipses for each contour
	vector<RotatedRect> minRect(v_contornos.size());
	vector<RotatedRect> minEllipse(v_contornos.size());

	for (int i = 0; i < v_contornos.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(v_contornos[i]));
		if (v_contornos[i].size() > 5)
		{
			minEllipse[i] = fitEllipse(Mat(v_contornos[i]));
		}
	}

	
	/// Draw contours + rotated rects + ellipses
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< v_contornos.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// contour
		//if(v_contornos[i]>brect[0])
		drawContours(drawing, v_contornos, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		// ellipse
		ellipse(drawing, minEllipse[i], color, 2, 8);
		// rotated rectangle
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}

	salida.push_back(drawing);
	//salida.push_back(image);
	//salida.push_back(recortar);
	/// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//imshow("Contours", drawing);



	return salida;

}
