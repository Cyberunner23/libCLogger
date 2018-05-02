/*
Copyright 2018 Alex Frappier Lachapelle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

template<class T>
class Singleton{

public:

    Singleton()                            = delete;
    ~Singleton()                           = delete;
    Singleton(Singleton const&)            = delete;
    Singleton& operator=(Singleton const&) = delete;

    static T* getInstance(){
        if(instance == nullptr){
            instance = std::unique_ptr<T>(new T());
        }
        return instance.get();
    }

    static void destroy(){
        instance.reset();
    }

private:

    static std::unique_ptr<T> instance;

};
template<class T> std::unique_ptr<T> Singleton<T>::instance = nullptr;
