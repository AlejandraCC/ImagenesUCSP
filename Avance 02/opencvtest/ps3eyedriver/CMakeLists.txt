

include_directories( ${OpenCV_INCLUDE_DIRS} deps/libusb/include/libusb-1.0 )
link_directories( deps/libusb/MS64/dll deps/libusb/MS64/static )

add_library( ps3eyecv
			 ps3eye.cpp
			 PSEyeVideoCapture.cpp )

target_link_libraries( ps3eyecv
					   ${OpenCV_LIBS}
					   libusb-1.0 )