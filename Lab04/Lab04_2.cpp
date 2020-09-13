// Example 9-2. Toy program for using a mouse to draw boxes on the screen
#include <opencv2/opencv.hpp>
#include <math.h>
int B = 0, G = 0, R = 0;
char shape = 'r';
bool size = 0;
char save = 'n';
char color;
cv::Point p;
// Define our callback which we will install for
// mouse events
//
void my_mouse_callback(

  int event, int x, int y, int flags, void* param
);
cv::Rect box;
bool drawing_box = false;
//cv::LineIterator circ;
bool drawing_circle = false;

// this function reads the global color variable and assigns the BGR values
void changeColor() {
	switch (color) {
	case 'k':
		B = 0;
		G = 0;
		R = 0;
		break;
	case 'r':
		B = 0;
		G = 0;
		R = 255;
		break;
	case 'g':
		B = 0;
		G = 255;
		R = 0;
		break;
	case 'b':
		B = 255;
		G = 0;
		R = 0;
		break;
	case 'y':
		B = 0;
		G = 255;
		R = 255;
		break;
	case 'w':
		B = 255;
		G = 255;
		R = 255;
		break;
	case 'm':
		B = 255;
		G = 0;
		R = 255;
		break;
	case 'c':
		B = 255;
		G = 255;
		R = 0;
		break;
	case 'x':
		srand(time(NULL));
		B = rand() % 256;
		G = rand() % 256;
		R = rand() % 256;
		break;
	default:
		break;
	}
}

// A little subroutine to draw a box onto an image
//
void draw_box( cv::Mat& img, cv::Rect box, int B, int G, int R, char shape ) {

	if (shape == 'r') {
		cv::rectangle(
			img,
			box.tl(),
			box.br(),

			cv::Scalar(B, G, R)
		);
	}
	else if (shape == 'c'){
		cv::circle(img, (box.tl() + box.br())/2, sqrt(box.width * box.width + box.height * box.height)/2, cv::Scalar(B, G, R), 2, 8, 0);
	}
	else if (shape == 'e') {
		cv::ellipse(img, p, cv::Size(abs(box.height), abs(box.width)), 90.0, 0.0, 360.0, cv::Scalar(B, G, R));
	}
	if (!drawing_box) {
		std::cout << "enter a color ";
		std::cin >> color;
		changeColor();
	}
}
void help(char** argv) {
  std::cout << "Example 9-2. Toy program for using a mouse to draw boxes on the screen"
     << "\nCall:\n" << argv[0] <<
    "\n\nshows how to use a mouse to draw regions in an image. Esc to quit\n" << std::endl;
}
int main( int argc, char** argv ) {

  help(argv);
  box = cv::Rect(-1,-1,0,0);

  // CHANGED to 480 x 640
  cv::Mat image(480, 640, CV_8UC3), temp;

  image.copyTo(temp);
  box   = cv::Rect(-1,-1,0,0);
  image = cv::Scalar::all(0);
  cv::namedWindow( "Box Example" );

  // moved window to (10, 30)
  cv::moveWindow("Box Example", 10, 30);

 // Here is the crucial moment where we actually install
 // the callback. Note that we set the value of 'params' to
 // be the image we are working with so that the callback
 // will have the image to edit.
 //
 cv::setMouseCallback(
   "Box Example",
   my_mouse_callback,
   (void*)&image
 );
 // The main program loop. Here we copy the working image
 // to the temp image, and if the user is drawing, then
 // put the currently contemplated box onto that temp image.
 // Display the temp image, and wait 15ms for a keystroke,
 // then repeat.
 

 int baseLine = 0;
 cv::Point textPoint = cv::Point(150, 40);
 // Writing Text
 cv::putText(image, "My Drawing Application", textPoint, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, cv::Scalar(255, 255, 255));

 // underlining text
 cv::Size textSize = getTextSize("My Drawing Application", cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1, 1, &baseLine);
 cv::line(image, textPoint, cv::Point(textPoint.x + textSize.width, 40), cv::Scalar(0, 0, 255));

 std::cout << "enter a color ";
 std::cin >> color;
 changeColor();

 for(;;) {
	 
    image.copyTo(temp);
	
    if( drawing_box ) draw_box( temp, box, B, G, R, shape );
    cv::imshow( "Box Example", temp );
	
	if (cv::waitKey(15) == 27) {
		// close window then ask if user wishes to save
		cv::destroyWindow("Box Example");
		std::cout << "Do you wish to save? ";
		std::cin >> save;
		if (save == 'y') {
			cv::imwrite("image.png", image);
			save = 'n';
		}
		break;
	};
  }
  return 0;
}

// This is our mouse callback. If the user
// presses the left button, we start a box.
// When the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//
void my_mouse_callback(
int event, int x, int y, int flags, void* param) 
{
  cv::Mat& image = *(cv::Mat*) param;
  switch( event ) {
    case cv::EVENT_MOUSEMOVE: {
      if( drawing_box ) {
        box.width  = x-box.x;
        box.height = y-box.y;
	  }
    }
    break;
	case cv::EVENT_LBUTTONDOWN: {
	  drawing_box = true;
	  box = cv::Rect(x, y, 0, 0);
	  p = cv::Point(x, y);

	  // check which shape to draw
	  if (cv::EVENT_FLAG_SHIFTKEY & flags) {
		  shape = 'c';
	  }
	  else if (cv::EVENT_FLAG_CTRLKEY & flags) {
		  shape = 'e';
	  }
	  else {
		  shape = 'r';
	  }
    }
    break;
    case cv::EVENT_LBUTTONUP: {
      drawing_box = false;
      if( box.width < 0  ) {

        box.x += box.width;
        box.width *= -1;
      }
      if( box.height < 0 ) {
        box.y += box.height;
        box.height *= -1;
      }
      draw_box( image, box, B, G, R, shape);
    }
    break;
  }
}