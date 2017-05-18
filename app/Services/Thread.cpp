#include "Thread.h"

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>

namespace App { namespace Services
{

    /**
     * Provides a wrapper to allow threads to run continuously and shut down correctly
     * This is thread safe!
     *
     * @brief Thread::Thread
     * @param parent
     * @param working
     * @param abort
     */
    Thread::Thread(QObject *parent, bool working, bool abort)
        : thread_abort(abort),
          thread_working(working)
    {
    }

    /**
     * Setup the connections
     *
     * @brief Thread::setup
     * @param thread
     */
    void Thread::setup(QThread &thread)
    {
        // Connect the work requested method to when the thread starts
        connect(this, SIGNAL(workRequested()), &thread, SLOT(start()));

        // Conect the thread started to the worker
        connect(&thread, SIGNAL(started()), this, SLOT(foreverLoop())); //worker

        // Connect the finish work signal to the thread quit method
        connect(this, SIGNAL(finished()), &thread, SLOT(quit()), Qt::DirectConnection);

        // Run any configuration on the child class
        configure();
    }

    /**
     * Tells the worker to start running
     * @brief Thread::requestWork
     */
    void Thread::requestWork()
    {
        // Lock the object
        mutex.lock();

        // Set the working var and abort var
        thread_working = true;
        thread_abort = false;

        // Send out a debug notice
        qDebug() << "Started a Thread: " << thread()->currentThreadId();

        // Unlock the object
        mutex.unlock();

        // Send out signal to start the worker
        emit workRequested();
    }

    /**
     * Tells the worker to stop working
     * @brief Thread::abort
     */
    void Thread::abort()
    {
        // Lock the object
        mutex.lock();

        // If the thread is currently running
        if (thread_working)
        {
            // Tell the working to stop
            thread_abort = true;

            // Send out a debug notice
            qDebug() << "Aborting the thread: " << thread()->currentThreadId();
        }

        // Unlock the object
        mutex.unlock();
    }


    /**
     * Keeps the thread running in a forever loop while having the ability to
     * end the thread at any time via the thread_abort member variable
     *
     * @brief Thread::foreverLoop
     */
    void Thread::foreverLoop()
    {
        qDebug() << "Working is running in: " << thread()->currentThreadId();

        while(true) {

            // Lock the object
            mutex.lock();

            // Cache the status of whether to abort
            bool abort = thread_abort;

            // Unlock the object
            mutex.unlock();

            // Check if we need to abort the thread
            if (abort)
            {
                // Send out a debug notice
                qDebug() << "Aborting a worker in thread: " << thread()->currentThreadId();

                // Break out of loop
                break;
            }

            // Runn the worker
            worker();
        }

        // Lock the object
        mutex.lock();

        // Set the worker as shut down
        thread_working = false;

        // Unlock the object
        mutex.unlock();

        // Send out a debug notice
        qDebug() << "Worker process has finished in thread: " << thread()->currentThreadId();

        // Tell the app that the process has finished
        emit finished();
    }

}}
