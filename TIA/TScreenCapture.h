#include <iostream>
#include <windows.h>
#include <vector>
#include <winapifamily.h>
#include <objidl.h>
#include <gdiplus.h>
#include <fstream>
#include <atlimage.h>
#include <opencv2/opencv.hpp>

using namespace Gdiplus;

namespace ScreenCapture {
    BITMAPINFOHEADER createBitmapHeader(int width, int height)
    {
        BITMAPINFOHEADER  bi;

        // create a bitmap
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;  //this is the line that makes it draw upside down or not
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;

        return bi;
    }

    HBITMAP GdiPlusScreenCapture(HWND hWnd)
    {
        // get handles to a device context (DC)
        HDC hwindowDC = GetDC(hWnd);
        HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
        SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

        // define scale, height and width
        int scale = 1;
        int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
        int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

        // create a bitmap
        HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
        BITMAPINFOHEADER bi = createBitmapHeader(width, height);

        // use the previously created device context with the bitmap
        SelectObject(hwindowCompatibleDC, hbwindow);

        // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that call HeapAlloc using a handle to the process's default heap.
        // Therefore, GlobalAlloc and LocalAlloc have greater overhead than HeapAlloc.
        DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
        HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
        char* lpbitmap = (char*)GlobalLock(hDIB);

        // copy from the window device context to the bitmap device context
        StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);   //change SRCCOPY to NOTSRCCOPY for wacky colors !
        GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        // avoid memory leak
        DeleteDC(hwindowCompatibleDC);
        ReleaseDC(hWnd, hwindowDC);
        return hbwindow;
    }

    bool saveToMemory(HBITMAP* hbitmap, std::vector<BYTE>& data, std::string dataFormat = "png")
    {
        
        Bitmap bmp(*hbitmap, nullptr);
        // write to IStream
        IStream* istream = nullptr;
        CreateStreamOnHGlobal(NULL, TRUE, &istream);

        // define encoding
        CLSID clsid;
        if (dataFormat.compare("bmp") == 0) { CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
        else if (dataFormat.compare("jpg") == 0) { CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
        else if (dataFormat.compare("gif") == 0) { CLSIDFromString(L"{557cf402-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
        else if (dataFormat.compare("tif") == 0) { CLSIDFromString(L"{557cf405-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
        else if (dataFormat.compare("png") == 0) { CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid); }

        Gdiplus::Status status = bmp.Save(istream, &clsid, NULL);
        if (status != Gdiplus::Status::Ok)
            return false;

        // get memory handle associated with istream
        HGLOBAL hg = NULL;
        GetHGlobalFromStream(istream, &hg);

        // copy IStream to buffer
        int bufsize = GlobalSize(hg);
        data.resize(bufsize);

        // lock & unlock memory
        LPVOID pimage = GlobalLock(hg);
        memcpy(&data[0], pimage, bufsize);
        GlobalUnlock(hg);
        istream->Release();
        return true;
    }



    bool temp() {
        // Initialize GDI+.
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        // get the bitmap handle to the bitmap screenshot
        HWND hWnd = GetDesktopWindow();
        HBITMAP hBmp = GdiPlusScreenCapture(hWnd);

        // save as png to memory
        std::vector<BYTE> data;
        std::string dataFormat = "bmp";

        if (saveToMemory(&hBmp, data, dataFormat))
        {
            std::wcout << "Screenshot saved to memory" << std::endl;

            // save from memory to file
            std::ofstream fout("C:/MyWorkspace/images/Screenshot-m2.png" + dataFormat, std::ios::binary);
            fout.write((char*)data.data(), data.size());
        }
        else
            std::wcout << "Error: Couldn't save screenshot to memory" << std::endl;


        // save as png (method 2)
        CImage image;
        image.Attach(hBmp);
        image.Save("C:/MyWorkspace/images/Screenshot-m2.png");

        GdiplusShutdown(gdiplusToken);
        return true;
    }



    cv::Mat getMat(HWND hWND) {
        /*
        * https://www.youtube.com/watch?v=mwfHopNiA3I
        */
        HDC deviceContext = GetDC(hWND);
        HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);

        RECT windowRect;
        GetClientRect(hWND, &windowRect);

        int height = windowRect.bottom;
        int width = windowRect.right;

        HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);

        SelectObject(memoryDeviceContext, bitmap);

        // copy data into bitmap
        BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);

        // specify format by using bitmapinfoheader
        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 1;
        bi.biYPelsPerMeter = 2;
        bi.biClrUsed = 3;
        bi.biClrImportant = 4;

        cv::Mat mat = cv::Mat(height, width, CV_8UC4); // 8 bit unsigned ints 4 channels -> RBGA

        // Transform data and store into mat.data
        GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        //Clean up!
        DeleteObject(bitmap);
        DeleteDC(memoryDeviceContext);
        ReleaseDC(hWND, deviceContext);

        return mat;
    }

    int mainy() {

        LPCSTR windowTitle = "INSERT TITLE HERE";
        HWND hWND = FindWindow(NULL, windowTitle);
        while (!hWND) {
            std::system("cls");
            hWND = FindWindow(NULL, windowTitle);
            Sleep(10);
        }

        cv::namedWindow("output", cv::WINDOW_NORMAL);

        while (true) {
            cv::Mat target = getMat(hWND);
            
            cv::Mat background;
            target.copyTo(background);

            cv::cvtColor(target, target, cv::COLOR_BGR2HSV);

            cv::rectangle(target, cv::Point(0, 0), cv::Point(640, 30), CV_RGB(0, 0, 0), cv::FILLED);

            cv::imshow("output", background);
            cv::waitKey(0);
        }
    }


}