# video-conferencing-example


## 참고 사이트

* 동영상 설명 [https://www.youtube.com/playlist?list=PL_K0yFEgjop84hh7tdSKQB-NXBqkAwSSA](https://www.youtube.com/playlist?list=PL_K0yFEgjop84hh7tdSKQB-NXBqkAwSSA)
* 참고 사이트 [http://10bun.tv/](http://10bun.tv/)


## 필요한 라이브러리

### 오픈소스 라이브러리

* boost-asio
* libvpx
* opus
* portaudio
* opencv4

> vcpkg를 사용하시는 경우라면 아래와 같이 설치하시면 됩니다.
> C:\...> vcpkg install boost-asio libvpx opus portaudio opencv4


### RyuLib for C++

소켓 등 기본적인 라이브러리는 "RyuLib for C++"에 미리 구현된 것을 사용할 것입니다.
소스는 아래 링크에 있습니다.

* [https://github.com/ryujt/ryulib-cpp](https://github.com/ryujt/ryulib-cpp)

이 프로젝트(video-conferencing-example) 소스에 submodule로 포함되어 있기 때문에 굳이 다시 내려 받을 필요는 없습니다.

