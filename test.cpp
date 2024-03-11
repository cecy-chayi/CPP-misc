while (true)
{
    {
        std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
        if (m_pool->m_shutdown) // 将退出操作放到锁后面，保证shutdown更改后可以正常退出
        {
            break;
        }
        if (m_pool->m_queue.empty())
        {
            m_pool->m_conditional_lock.wait(lock);
        }
        dequeued = m_pool->m_queue.dequeue(func);
    }
    if (dequeued)
    {
        (*func)();
    }
}

void shutdown()
{
    {
        std::unique_lock<std::mutex> lock(m_conditional_mutex); // 对shutdown加锁
        m_shutdown = true;
        m_conditional_lock.notify_all();
    }

    for (int i = 0; i < m_threads.size(); ++i)
    {
        if (m_threads[i].joinable())
        {
            m_threads[i].join();
        }
    }
}