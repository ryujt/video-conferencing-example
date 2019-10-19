#pragma once
#pragma comment(lib, "strmiids")
#pragma comment(lib, "comsuppw.lib")  


#include <windows.h>
#include <dshow.h>
#include <string>
#include <vector>
#include <comutil.h>  


using namespace std;


class CameraList
{
private:
	vector<string> names_;

	HRESULT enumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
	{
		ICreateDevEnum *pDevEnum;
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
		if (SUCCEEDED(hr)) {
			hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
			if (hr == S_FALSE) hr = VFW_E_NOT_FOUND;
			pDevEnum->Release();
		}
		return hr;
	}

	void displayDeviceInformation(IEnumMoniker *pEnum)
	{
		names_.clear();

		IMoniker *pMoniker = NULL;
		while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
			IPropertyBag *pPropBag;
			HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
			if (FAILED(hr)) {
				pMoniker->Release();
				continue;
			}

			VARIANT var;
			VariantInit(&var);

			hr = pPropBag->Read(L"Description", &var, 0);
			if (FAILED(hr)) {
				hr = pPropBag->Read(L"FriendlyName", &var, 0);
			}
			if (SUCCEEDED(hr)) {
				names_.push_back(_com_util::ConvertBSTRToString(var.bstrVal) );
				VariantClear(&var);
			}

			pPropBag->Release();
			pMoniker->Release();
		}
	}
public:
	CameraList()
		: names_()
	{

	}

	void update()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (FAILED(hr)) return;

		IEnumMoniker *pEnum;
		hr = enumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
		if (SUCCEEDED(hr)) {
			displayDeviceInformation(pEnum);
			pEnum->Release();
		}

		CoUninitialize();
	}

	int size() { return names_.size();  }

	string getName(int index) { return names_.at(index);  }
};
