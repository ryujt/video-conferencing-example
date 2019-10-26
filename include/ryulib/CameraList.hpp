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
public:
	/** 카메라 목록을 갱신합니다.
	*/
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

	/** 찾아낸 카메라의 개수입니다.
	@return 카메라 개수
	*/
	int size() { return names_.size();  }

	/** 카메라의 이름을 가져옵니다.
	@param index 가져올 카메라의 순번 (순번은 0부터 시작함)
	*/
	string getName(int index) { return names_.at(index);  }

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
};
