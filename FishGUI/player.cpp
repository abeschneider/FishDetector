#include <player.h>

Player::Player(QObject *parent)
	: QThread(parent)
{
    frameIndex = 0;
	stop = true;
}

bool Player::loadVideo(std::string filename) {
    capture.reset(new cv::VideoCapture(filename));
	if (capture->isOpened())
	{
		frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        currentSpeed = 100;
		delay = (1000/frameRate);
		return true;
	}
	else
		return false;
}

void Player::Play()
{
	if (!isRunning()) 
	{
		if (isStopped())
		{
			stop = false;
		}
		start(LowPriority);
	}
}

void Player::run()
{
	while(!stop) {
		if (!capture->read(frame))
		{
			stop = true;
		}
        frameIndex++;
		if (frame.channels() == 3)
		{
			cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
			img = QImage((const unsigned char*)(RGBframe.data),
				RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
		}
		else
		{
			img = QImage((const unsigned char*)(frame.data),
				frame.cols, frame.rows, QImage::Format_Indexed8);
		}
		emit processedImage(img);
		this->msleep(delay);
	}
}

QImage Player::getOneFrame()
{
	capture->read(frame);
	if (frame.channels() == 3)
	{
		cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
		img = QImage((const unsigned char*)(RGBframe.data),
			RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
	}
	else
	{
		img = QImage((const unsigned char*)(frame.data),
			frame.cols, frame.rows, QImage::Format_Indexed8);
	}

	return img;
}

Player::~Player()
{
    if (1)
    {
        QMutexLocker locker(&mutex);
        stop = true;
        //capture->release();
        condition.wakeOne();
        wait();
    }
}
void Player::Stop()
{
	stop = true;
}

void Player::msleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::microseconds(ms*1000));
}

bool Player::isStopped() const{
	return this->stop;
}

void Player::speedUp()
{
	delay = delay/2;
	if (delay<1)
	  delay = 1;
    currentSpeed = currentSpeed * 2;
}

void Player::slowDown()
{
	delay = delay*2;
    currentSpeed = currentSpeed / 2;
}

QImage Player::nextFrame() {
    setCurrentFrame(++frameIndex);
    return getOneFrame();
}

QImage Player::prevFrame() {
    setCurrentFrame(--frameIndex);
    return getOneFrame();
}


//void Player::minusOneFrame()
//{
//	std::int64_t currentFrame = getCurrentFrame() - 1;
////	currentFrame = currentFrame - (double)2;
//	setCurrentFrame(currentFrame);
//}

std::int64_t Player::getCurrentFrame()
{
//    double value = capture->get(CV_CAP_PROP_POS_FRAMES);
//    std::cout << "value = " << value << std::endl;
//	return (std::uint64_t)value;
    return frameIndex;
}

double Player::getNumberOfFrames()
{
	return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

double Player::getFrameRate()
{
	return frameRate;
}

void Player::setCurrentFrame(std::int64_t frameNumber)
{
    capture->set(CV_CAP_PROP_POS_MSEC, int((double(frameNumber)/double(frameRate))*1000));
    frameIndex = frameNumber;
//	capture->set(CV_CAP_PROP_POS_FRAMES, 2.0);
//    std::cout << "frame: " << frameNumber*1000 << ", actual: " << capture->get(CV_CAP_PROP_POS_MSEC) << std::endl;
}

QImage Player::setFrame(std::int64_t frame)
{
    setCurrentFrame(frame);
    return getOneFrame();
}


double Player::getCurrentSpeed()
{
    return currentSpeed;
}
