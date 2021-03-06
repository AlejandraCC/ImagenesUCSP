//
//
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <stdlib.h>
//#include <stdio.h>
//#include <opencv2/core/core.hpp>
//#include "opencv2/opencv.hpp"
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//#include <algorithm>
//
//using namespace cv;
//using namespace std;
//
//bool orden_X(Point a , Point b) {
//
//	return a.x < b.x;
//
//}
//bool orden_Y(Point a, Point b) {
//
//	return a.y > b.y;
//
//}
//
//int main(int argc, const char** argv)
//{
//	Mat img = imread("1.PNG", CV_LOAD_IMAGE_COLOR); // abrimos y leemos la imagen
//
//	if (img.empty()) //pregunta si la imagen fue recibida correctamente
//	{
//		cout << "Error : Imagen no pudo ser abierta!!" << endl;
//		//system("pause"); //wait for a key press
//		return -1;
//	}
//
//	namedWindow("Original", CV_WINDOW_AUTOSIZE); //creamos una ventana
//	imshow("Original", img); //mostramos la imagen original
//
//	cvtColor(img, img, CV_BGR2GRAY); // cambiamos la imagen a escala de grises
//	threshold(img, img, 50, 255, THRESH_BINARY); // umbralizacion 
//	//	adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 41);
//												 //
//	namedWindow("Gris", CV_WINDOW_AUTOSIZE); //creamos una ventana
//	imshow("Gris", img); //mostramos la imagen original
//
//
//
//	Mat imageContrastHigh2;
//	img.convertTo(imageContrastHigh2, -1, 1.15, 0); //increase the contrast by 2
//	String windowNameContrastHigh2 = "Contraste1";
//	namedWindow(windowNameContrastHigh2, WINDOW_NORMAL);
//	imshow(windowNameContrastHigh2, imageContrastHigh2);
//
//	//contraste de la imagen
//	//	Mat imgL = imageContrastHigh2 + Scalar(-75, -75, -75); //decrementa el contraste de la imagen en 75 unidadades a acada pixel  son 3 canales por BGR
//
//	//img.convertTo(imgL, -1, 1, -75);
//
//	//	namedWindow("contraste", CV_WINDOW_AUTOSIZE);
//	//	imshow("contraste", imgL);
//
//
//	// deteccion de bordes
//
//	Mat canny_output;
//	Canny(imageContrastHigh2, canny_output, 100, 100 * 2); // Detectar los bordes con un umbral min = 100 y max = 200
//	imshow("Bordes", canny_output);
//
//	vector<vector<Point> > v_contornos;
//	vector<Vec4i> hierarchy;
//	//// Buscar los contornos de la imagen, se almacenan en contours
//	findContours(canny_output, v_contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	//Mostrar la Imagen modificada por la funcion findContours
//	imshow("Modificada", canny_output);
//	Mat m_bordes = Mat::zeros(canny_output.size(), CV_8UC3);
//	for (size_t i = 0; i< v_contornos.size(); i++)
//	{
//		Scalar color = CV_RGB(50, 50, 0);
//		drawContours(m_bordes, v_contornos, (int)i, color, 2, 8, hierarchy, 0, Point());
//	}
//
//	// Mostrar la imagen final
//	//imshow("Contornos", m_bordes);
//
//
//	vector<Vec3f> circles;
//	vector<Point> v_circ;
//	//HoughCircles(canny_output, circles, CV_HOUGH_GRADIENT,	2, canny_output.rows / 4, 200, 100);
//	HoughCircles(canny_output, circles, HOUGH_GRADIENT, 1, 10, 100, 30, 1, 35 // change the last two parameters
//																			  // (min_radius & max_radius) to detect larger circles
//	);
//	for (size_t i = 0; i < circles.size(); i++)
//	{
//		Point n_c;
//		n_c.x = circles[i][0];
//		n_c.y = circles[i][1];
//		v_circ.push_back(n_c);
//
//		cout << circles[i][0] << " " << circles[i][1] << " " << circles[i][2] << " " << endl;
//		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//		int radius = cvRound(circles[i][2]);
//		// draw the circle center
//		circle(m_bordes, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//		// draw the circle outline
//		//circle(m_bordes, center, radius, Scalar(0, 0, 255), 3, 8, 0);
//	}
//	namedWindow("circles", 1);
//	imshow("circles", m_bordes);
//
//	
//	vector<RotatedRect> minRect(v_contornos.size());
//	vector<RotatedRect> minEllipse(v_contornos.size());
//
//	for (int i = 0; i < v_contornos.size(); i++)
//	{
//		minRect[i] = minAreaRect(Mat(v_contornos[i]));
//		if (v_contornos[i].size() > 5)
//		{
//			minEllipse[i] = fitEllipse(Mat(v_contornos[i]));
//		}
//	}
//
//	RNG rng(12345);
//	/// Draw contours + rotated rects + ellipses
//	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
//	for (int i = 0; i< v_contornos.size(); i++)
//	{
//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//		// contour
//		drawContours(drawing, v_contornos, i, color, 1, 8, vector<Vec4i>(), 0, Point());
//		// ellipse
//		ellipse(drawing, minEllipse[i], color, 2, 8);
//		// rotated rectangle
//		Point2f rect_points[4]; minRect[i].points(rect_points);
//		for (int j = 0; j < 4; j++)
//			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
//	}
//
//	/// Show in a window
//	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
//	imshow("Contours", drawing);
//	//vector<Vec3f> v_circle=circles;
//
//
//	std::vector<cv::Point> poly=v_circ;
//	cv::RotatedRect box = cv::minAreaRect(poly); // now it works!
//
//	cv::Point2f corners[4];
//	box.points(corners);
//	Mat image = canny_output;
//	// Draw the bounding box
//	for (int i = 0; i < 4; i++) {
//		line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);
//	//	line(image, corners[i], corners[(i + 1) % 4], Scalar(0, 255, 0));
//	}
//	//Rect brect = box.boundingRect();
//	//rectangle(image, brect, Scalar(255, 0, 0));
//	//imshow("rectangles", image);
//
//	imshow("Rectangle, triangle & circle", canny_output);
//
//	/*RotatedRect rRect = RotatedRect(box);
//	Rect brect = rRect.boundingRect();
//	rectangle(canny_output, brect, Scalar(255, 0, 0));
//	imshow("rectangles", canny_output);*/
//
//
//	/*Mat image=img;
//	RotatedRect rRect = RotatedRect(box);
//	Point2f vertices[4];
//	rRect.points(vertices);
//	for (int i = 0; i < 4; i++)
//		line(image, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
//	Rect brect = rRect.boundingRect();
//	rectangle(image, brect, Scalar(255, 0, 0));
//	imshow("rectangles", image);
//*/
//
//	sort(v_circ.begin(), v_circ.end(), orden_X);
//
//	vector<vector<Point>>  v_cir_sa;
//	for (size_t j = 0; j < 5; j++) {
//		vector<Point> v_po;
//
//		for (size_t i = j*4; i < j*4+4 ; i++)
//		{
//			
//			Point p_p;
//			p_p.x = v_circ[i].x;
//			p_p.y = v_circ[i].y;
//
//			v_po.push_back(p_p);
//
//			cout << v_circ[i].x << " " << v_circ[i].y << " " << endl;
//		}
//		sort(v_po.begin(), v_po.end(),orden_Y);
//		v_cir_sa.push_back(v_po);
//	}
//
//	for (size_t i = 0; i < 4; i++)
//	{
//		for (size_t j = 0; j < 3; j++)
//		{
//			cout << v_cir_sa[i][j].x << " " << v_cir_sa[i][j].y << " " << endl;
//			line(m_bordes, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
//			line(canny_output, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
//			//line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);
//			
//		}
//		cout << v_cir_sa[i][3].x << " " << v_cir_sa[i][3].y << " " << endl;
//		line(m_bordes, v_cir_sa[i][3], v_cir_sa[i+1][0], Scalar(255, 100, 255), 1, LINE_AA);
//		line(canny_output, v_cir_sa[i][3], v_cir_sa[i + 1][0], Scalar(255, 100, 255), 1, LINE_AA);
//	}
//	line(m_bordes, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
//	line(m_bordes, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
//	line(m_bordes, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);
//
//	line(canny_output, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
//	line(canny_output, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
//	line(canny_output, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);
//
//	imshow("Recircle", m_bordes);
//	imshow("Recircle2", canny_output);
//
//	waitKey(0); //wait infinite time for a keypress
//
//	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"
//
//
//
//	return 0;
//}

/* #include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{
//open the video file for reading
VideoCapture cap("D:/UCSP/imagenes/manuel/realsense_Depth.avi");

// if not success, exit program
if (cap.isOpened() == false)
{
cout << "Cannot open the video file" << endl;
cin.get(); //wait for any key press
return -1;
}

//Uncomment the following line if you want to start the video in the middle
//cap.set(CAP_PROP_POS_MSEC, 300);

//get the frames rate of the video
double fps = cap.get(CAP_PROP_FPS);
cout << "Frames per seconds : " << fps << endl;




Mat edges;
namedWindow("edges", 1);
for (;;)
{
Mat frame;
cap >> frame; // get a new frame from camera
cvtColor(frame, edges, COLOR_BGR2GRAY);
GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
Canny(edges, edges, 0, 30, 3);
imshow("edges", edges);
if (waitKey(10) == 27) break;
}


return 0;

}
*/

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp> 
#include<opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/videoio.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

vector<Mat> preprocesamiento(Mat entrada);
bool orden_X(Point a, Point b) {

	return a.x < b.x;

}
bool orden_Y(Point a, Point b) {

	return a.y > b.y;

}



int main(int argc, char** argv)
{
	//open the video file for reading
	VideoCapture cap("C:/Users/Alesx/Documents/manues/calibration_ps3eyecam.avi");
	//VideoCapture cap(0);
	// if not success, exit program
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	//Defining window names
	String windowNameOriginalVideo = "Original Video";
	String windowNameBrightnessHigh50 = "Brightness Increased by 50";
	String windowNameContrastHigh2 = "Contrast Increased by 2";

	//Create and open windows for above window names
	namedWindow(windowNameOriginalVideo, WINDOW_NORMAL);
	namedWindow(windowNameBrightnessHigh50, WINDOW_NORMAL);
	namedWindow(windowNameContrastHigh2, WINDOW_NORMAL);



	while (true)
	{
		Mat frame;
		//cap.read(frame);
		bool bSuccess = cap.read(frame); // read a new frame from video 

										 //Breaking the while loop at the end of the video
		if (bSuccess == false)
		{
			cout << "Found the end of the video" << endl;
			break;
		}
		//imshow("cam", frame);
		vector<Mat> salida0;
		salida0= preprocesamiento(frame);
		/*
		namedWindow("circles0", WINDOW_NORMAL);
		imshow("circles0", salida0[0]);
		namedWindow("R-c", WINDOW_NORMAL);
		imshow("R-c", salida0[1]);
		*/
		namedWindow("Rc", WINDOW_NORMAL);
		imshow("Rc", salida0[2]);
		//namedWindow("cc", WINDOW_NORMAL);
		//imshow("cc", salida0[3]);

		Mat frameBrighnessHigh50;
		frame.convertTo(frameBrighnessHigh50, -1, 1, 50); //increase the brightness by 50
														  //frame.convertTo(frameBrighnessHigh50, -1, 1, -50); //decrease the brightness by 50
		cvtColor(frameBrighnessHigh50, frameBrighnessHigh50, CV_BGR2GRAY);
		//threshold(frameBrighnessHigh50, frameBrighnessHigh50, 50, 255, THRESH_BINARY);

		vector<Mat> salida1 = preprocesamiento(frameBrighnessHigh50);
		/*
		namedWindow("circles", WINDOW_NORMAL);
		imshow("circles", salida1[0]);
		namedWindow("R-c1", WINDOW_NORMAL);
		imshow("R-c1", salida1[1]);
		*/
		namedWindow("Rc1", WINDOW_NORMAL);
		imshow("Rc1", salida1[2]);
		//namedWindow("cc1", WINDOW_NORMAL);
	//	imshow("cc1", salida1[3]);

		Mat frameContrastHigh2;
		frame.convertTo(frameContrastHigh2, -1, 2, 0); //increase the contrast by 2
		cvtColor(frameContrastHigh2, frameContrastHigh2, CV_BGR2GRAY);
		//threshold(frameContrastHigh2, frameContrastHigh2, 200, 255, THRESH_BINARY);
		vector<Mat> salida2 = preprocesamiento(frameContrastHigh2);
		/*
		namedWindow("circles2", WINDOW_NORMAL);
		imshow("circles2", salida2[0]);
		namedWindow("R-c2", WINDOW_NORMAL);
		imshow("R-c2", salida2[1]);
		*/
		namedWindow("Rc2", WINDOW_NORMAL);
		imshow("Rc2", salida2[2]);
	//	namedWindow("cc2", WINDOW_NORMAL);
		//imshow("cc2", salida2[3]);

		imshow(windowNameOriginalVideo, frame);
		imshow(windowNameBrightnessHigh50, frameBrighnessHigh50);
		imshow(windowNameContrastHigh2, frameContrastHigh2);


		if (waitKey(30) >= 0)
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
		Point2f n_c;
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
	

	///// Find the rotated rectangles and ellipses for each contour
	//vector<RotatedRect> minRect(v_contornos.size());
	//vector<RotatedRect> minEllipse(v_contornos.size());

	//for (int i = 0; i < v_contornos.size(); i++)
	//{
	//	minRect[i] = minAreaRect(Mat(v_contornos[i]));
	//	if (v_contornos[i].size() > 5)
	//	{
	//		minEllipse[i] = fitEllipse(Mat(v_contornos[i]));
	//	}
	//}

	//std::vector<cv::Point> poly = v_circ;
	//cv::RotatedRect box = cv::minAreaRect(poly); // now it works!


	//cv::Point2f corners[4];
	//box.points(corners);
	//Mat image = canny_output;
	//// Draw the bounding box
	//for (int i = 0; i < 4; i++) {
	//	line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);
	//	//	line(image, corners[i], corners[(i + 1) % 4], Scalar(0, 255, 0));
	//}
	//Rect brect = box.boundingRect();
	//rectangle(image, brect, Scalar(255, 0, 0));



	//imshow("rectangles", image);

	//imshow("Rectangle, triangle & circle", canny_output);

	//Mat recortar;
	//Mat rec = image(brect);
	//resize(rec, recortar, Size(512, 512), 1.0, 1.0, INTER_CUBIC);


	/// Draw contours + rotated rects + ellipses
	//Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	//for (int i = 0; i< v_contornos.size(); i++)
	//{
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	// contour
	//	//if(v_contornos[i]>brect[0])
	//	drawContours(drawing, v_contornos, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	//	// ellipse
	//	ellipse(drawing, minEllipse[i], color, 2, 8);
	//	// rotated rectangle
	//	Point2f rect_points[4]; minRect[i].points(rect_points);
	//	for (int j = 0; j < 4; j++)
	//		line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	//}

	sort(v_circ.begin(), v_circ.end(), orden_X);
	
	int aa = (v_circ.size() / 4);
	vector<vector<Point>>  v_cir_sa;
	for (size_t j = 0; j < aa; j++) {
		vector<Point> v_po;

		for (size_t i = j * 4; i < j * 4 + 4; i++)
		{

			Point p_p;
			p_p.x = v_circ[i].x;
			p_p.y = v_circ[i].y;

			v_po.push_back(p_p);

			cout << v_circ[i].x << " " << v_circ[i].y << " " << endl;
		}
		sort(v_po.begin(), v_po.end(), orden_Y);
		v_cir_sa.push_back(v_po);
	}

	int  a = v_cir_sa.size();
	int b = v_cir_sa[a-1].size();

	for (size_t i = 0; i <a-1; i++)
	{
		for (size_t j = 0; j < b-1; j++)
		{
			//cout << v_cir_sa[i][j].x << " " << v_cir_sa[i][j].y << " " << endl;
			line(m_bordes, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
			line(canny_output, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
			//line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);

		}
		//cout << v_cir_sa[i][3].x << " " << v_cir_sa[i][3].y << " " << endl;
		line(m_bordes, v_cir_sa[i][b-1], v_cir_sa[i + 1][0], Scalar(255, 100, 255), 1, LINE_AA);
		line(canny_output, v_cir_sa[i][b-1], v_cir_sa[i + 1][0], Scalar(255, 100, 255), 1, LINE_AA);
	}

	for (size_t i = 0; i < b-1; i++)
	{
		line(m_bordes, v_cir_sa[a-1][i], v_cir_sa[a-1][i+1], Scalar(255, 100, 255), 1, LINE_AA);
		line(canny_output, v_cir_sa[a - 1][i], v_cir_sa[a - 1][i+1], Scalar(255, 100, 255), 1, LINE_AA);
	}
	/*line(m_bordes, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
	line(m_bordes, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
	line(m_bordes, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);

	line(canny_output, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
	line(canny_output, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
	line(canny_output, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);*/


	Mat image = m_bordes;

	salida.push_back(m_bordes);
	//salida.push_back(drawing);
	salida.push_back(image);
	salida.push_back(m_bordes);
	salida.push_back(canny_output);
	//salida.push_back(cimage);
	//salida.push_back(recortar);
	/// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//imshow("Contours", drawing);



	return salida;

}
