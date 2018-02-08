#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

bool orden_X(Point a , Point b) {

	return a.x < b.x;

}
bool orden_Y(Point a, Point b) {

	return a.y > b.y;

}

int main(int argc, const char** argv)
{
	Mat img = imread("1.PNG", CV_LOAD_IMAGE_COLOR); // abrimos y leemos la imagen

	if (img.empty()) //pregunta si la imagen fue recibida correctamente
	{
		cout << "Error : Imagen no pudo ser abierta!!" << endl;
		//system("pause"); //wait for a key press
		return -1;
	}

	namedWindow("Original", CV_WINDOW_AUTOSIZE); //creamos una ventana
	imshow("Original", img); //mostramos la imagen original

	cvtColor(img, img, CV_BGR2GRAY); // cambiamos la imagen a escala de grises
	threshold(img, img, 50, 255, THRESH_BINARY); // umbralizacion 
												 //adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 41);
												 //
	namedWindow("Gris", CV_WINDOW_AUTOSIZE); //creamos una ventana
	imshow("Gris", img); //mostramos la imagen original



	Mat imageContrastHigh2;
	img.convertTo(imageContrastHigh2, -1, 1.15, 0); //increase the contrast by 2
	String windowNameContrastHigh2 = "Contraste1";
	namedWindow(windowNameContrastHigh2, WINDOW_NORMAL);
	imshow(windowNameContrastHigh2, imageContrastHigh2);

	//contraste de la imagen
	//	Mat imgL = imageContrastHigh2 + Scalar(-75, -75, -75); //decrementa el contraste de la imagen en 75 unidadades a acada pixel  son 3 canales por BGR

	//img.convertTo(imgL, -1, 1, -75);

	//	namedWindow("contraste", CV_WINDOW_AUTOSIZE);
	//	imshow("contraste", imgL);


	// deteccion de bordes

	Mat canny_output;
	Canny(imageContrastHigh2, canny_output, 100, 100 * 2); // Detectar los bordes con un umbral min = 100 y max = 200
	imshow("Bordes", canny_output);

	vector<vector<Point> > v_contornos;
	vector<Vec4i> hierarchy;
	//// Buscar los contornos de la imagen, se almacenan en contours
	findContours(canny_output, v_contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Mostrar la Imagen modificada por la funcion findContours
	imshow("Modificada", canny_output);
	Mat m_bordes = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i< v_contornos.size(); i++)
	{
		Scalar color = CV_RGB(50, 50, 0);
		drawContours(m_bordes, v_contornos, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	// Mostrar la imagen final
	//imshow("Contornos", m_bordes);


	vector<Vec3f> circles;
	vector<Point> v_circ;
	//HoughCircles(canny_output, circles, CV_HOUGH_GRADIENT,	2, canny_output.rows / 4, 200, 100);
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
	namedWindow("circles", 1);
	imshow("circles", m_bordes);

	
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

	RNG rng(12345);
	/// Draw contours + rotated rects + ellipses
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< v_contornos.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// contour
		drawContours(drawing, v_contornos, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		// ellipse
		ellipse(drawing, minEllipse[i], color, 2, 8);
		// rotated rectangle
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	//vector<Vec3f> v_circle=circles;


	std::vector<cv::Point> poly=v_circ;
	cv::RotatedRect box = cv::minAreaRect(poly); // now it works!

	cv::Point2f corners[4];
	box.points(corners);
	Mat image = canny_output;
	// Draw the bounding box
	for (int i = 0; i < 4; i++) {
		line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);
	//	line(image, corners[i], corners[(i + 1) % 4], Scalar(0, 255, 0));
	}
	//Rect brect = box.boundingRect();
	//rectangle(image, brect, Scalar(255, 0, 0));
	//imshow("rectangles", image);

	imshow("Rectangle, triangle & circle", canny_output);

	/*RotatedRect rRect = RotatedRect(box);
	Rect brect = rRect.boundingRect();
	rectangle(canny_output, brect, Scalar(255, 0, 0));
	imshow("rectangles", canny_output);*/


	/*Mat image=img;
	RotatedRect rRect = RotatedRect(box);
	Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(image, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
	Rect brect = rRect.boundingRect();
	rectangle(image, brect, Scalar(255, 0, 0));
	imshow("rectangles", image);
*/

	sort(v_circ.begin(), v_circ.end(), orden_X);

	vector<vector<Point>>  v_cir_sa;
	for (size_t j = 0; j < 5; j++) {
		vector<Point> v_po;

		for (size_t i = j*4; i < j*4+4 ; i++)
		{
			
			Point p_p;
			p_p.x = v_circ[i].x;
			p_p.y = v_circ[i].y;

			v_po.push_back(p_p);

			cout << v_circ[i].x << " " << v_circ[i].y << " " << endl;
		}
		sort(v_po.begin(), v_po.end(),orden_Y);
		v_cir_sa.push_back(v_po);
	}

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			cout << v_cir_sa[i][j].x << " " << v_cir_sa[i][j].y << " " << endl;
			line(m_bordes, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
			line(canny_output, v_cir_sa[i][j], v_cir_sa[i][j + 1], Scalar(255, 0, 255), 1, LINE_AA);
			//line(canny_output, corners[i], corners[(i + 1) % 4], Scalar(255, 255, 0), 1, LINE_AA);
			
		}
		cout << v_cir_sa[i][3].x << " " << v_cir_sa[i][3].y << " " << endl;
		line(m_bordes, v_cir_sa[i][3], v_cir_sa[i+1][0], Scalar(255, 100, 255), 1, LINE_AA);
		line(canny_output, v_cir_sa[i][3], v_cir_sa[i + 1][0], Scalar(255, 100, 255), 1, LINE_AA);
	}
	line(m_bordes, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
	line(m_bordes, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
	line(m_bordes, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);

	line(canny_output, v_cir_sa[4][0], v_cir_sa[4][1], Scalar(255, 100, 255), 1, LINE_AA);
	line(canny_output, v_cir_sa[4][1], v_cir_sa[4][2], Scalar(255, 100, 255), 1, LINE_AA);
	line(canny_output, v_cir_sa[4][2], v_cir_sa[4][3], Scalar(255, 100, 255), 1, LINE_AA);

	imshow("Recircle", m_bordes);
	imshow("Recircle2", canny_output);

	waitKey(0); //wait infinite time for a keypress

	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"



	return 0;
}