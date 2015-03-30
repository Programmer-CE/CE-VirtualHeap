#ifndef CRAZYTHREAD_H
#define CRAZYTHREAD_H
#include <pthread.h>
#include <signal.h>

/**
 * @brief Clase superior no instanciable que representa a los thread del proyecto Crazy River Ride
 * la implementacion de esta clase esta inspirada en la clase que se puede apreciar el en siguiente link
 *
 *
 */
class CrazyThread
{
public:
   /**
    * @brief Constructor de la clase, es el constructor por defecto
    *
    */
   CrazyThread() {}
   /**
    * @brief Es el destructor virtual de la clase
    *
    */
   virtual ~CrazyThread() {}
   /**
    * @brief Es el metodo que se encarga de iniciar la ejecucion del pthread
    *
    * @return retorna true si el thread fue creado con exito
    */
   bool start()
   {
      return (_ThreadId = pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
   }


   /**
    * @brief Este metodo se ecarga de esperar a que el thread termine su ejecucion
    *
    */
   void stop()
   {
       //int ret = pthread_kill(_thread, 0);
      (void) pthread_join(_thread, NULL);
       //return _thread;
   }

protected:
   /**
    * @brief Es el metodo que puede ser sobrescrito por aquella clase hija que desee tener su propia
    * implemetancion del thread
    *
    */
   virtual void internalRun() = 0;

private:
   /**
    * @brief Este metodo es interno y por ende nadie que herede de el puede sobrescribirlo o tocarlo
    * la idea es encapsular un metodo que se encargue del trabajo sucio y hacer del pthread una estructura
    * mas sencilla y maejable
    *
    * @param This es el objeto mismo de la clase
    */
   static void * InternalThreadEntryFunc(void * This) {((CrazyThread *)This)->internalRun(); return NULL;}

   int _ThreadId;
   pthread_t _thread; /**< TODO es el thread que da paso a la ejecucion del metodo @link{internalRun} */
};

#endif // CRAZYTHREAD_H
