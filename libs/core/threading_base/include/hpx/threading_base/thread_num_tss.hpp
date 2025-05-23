//  Copyright (c) 2007-2025 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \page hpx::get_worker_thread_num, hpx::get_local_worker_thread_num, hpx::get_local_worker_thread_num, hpx::get_thread_pool_num, hpx::get_thread_pool_num
/// \headerfile hpx/runtime.hpp

#pragma once

#include <hpx/config.hpp>
#include <hpx/modules/errors.hpp>

#include <cstddef>

#include <hpx/config/warnings_prefix.hpp>

namespace hpx::threads::detail {

    /// Set the global thread id to thread local storage.
    HPX_CORE_EXPORT std::size_t set_global_thread_num_tss(
        std::size_t num) noexcept;

    /// Get the global thread id from thread local storage.
    HPX_CORE_EXPORT std::size_t get_global_thread_num_tss() noexcept;

    /// Set the local thread id to thread local storage.
    HPX_CORE_EXPORT std::size_t set_local_thread_num_tss(
        std::size_t num) noexcept;

    /// Get the local thread id from thread local storage.
    HPX_CORE_EXPORT std::size_t get_local_thread_num_tss() noexcept;

    /// Set the thread pool id to thread local storage.
    HPX_CORE_EXPORT std::size_t set_thread_pool_num_tss(
        std::size_t num) noexcept;

    /// Get the thread pool id from thread local storage.
    HPX_CORE_EXPORT std::size_t get_thread_pool_num_tss() noexcept;

    /// Holds the global and local thread numbers, and the pool number
    /// associated with the thread.
    struct thread_nums
    {
        std::size_t global_thread_num;
        std::size_t local_thread_num;
        std::size_t thread_pool_num;
    };

    HPX_CORE_EXPORT void set_thread_nums_tss(thread_nums const&) noexcept;
    HPX_CORE_EXPORT thread_nums get_thread_nums_tss() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    struct reset_tss_helper
    {
        explicit reset_tss_helper(std::size_t global_thread_num) noexcept
          : global_thread_num_(set_global_thread_num_tss(global_thread_num))
        {
        }

        reset_tss_helper(reset_tss_helper const&) = delete;
        reset_tss_helper(reset_tss_helper&&) = delete;
        reset_tss_helper& operator=(reset_tss_helper const&) = delete;
        reset_tss_helper& operator=(reset_tss_helper&&) = delete;

        ~reset_tss_helper()
        {
            set_global_thread_num_tss(global_thread_num_);
        }

        [[nodiscard]] constexpr std::size_t previous_global_thread_num()
            const noexcept
        {
            return global_thread_num_;
        }

    private:
        std::size_t global_thread_num_;
    };
}    // namespace hpx::threads::detail

namespace hpx {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current OS-thread running in the runtime
    ///        instance the current HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the OS-thread which
    /// executes the current HPX-thread.
    ///
    /// \note   The returned value is zero based and its maximum value is
    ///         smaller than the overall number of OS-threads executed (as
    ///         returned by \a get_os_thread_count().
    ///
    /// \note   This function needs to be executed on a HPX-thread. It will
    ///         fail otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_worker_thread_num() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current OS-thread running in the runtime
    ///        instance the current HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the OS-thread which
    /// executes the current HPX-thread.
    ///
    /// \param ec [in,out] this represents the error status on exit (obsolete,
    ///        ignored).
    ///
    /// \note   The returned value is zero based and its maximum value is
    ///         smaller than the overall number of OS-threads executed (as
    ///         returned by \a get_os_thread_count(). It will return -1 if the
    ///         current thread is not a known thread or if the runtime is not in
    ///         running state.
    ///
    /// \note   This function needs to be executed on a HPX-thread. It will
    ///         fail otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_worker_thread_num(error_code&) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current OS-thread running in the current
    ///        thread pool the current HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the OS-thread on the
    /// current thread pool which executes the current HPX-thread.
    ///
    /// \note The returned value is zero based and its maximum value is smaller
    ///       than the number of OS-threads executed on the current thread pool.
    ///       It will return -1 if the current thread is not a known thread or
    ///       if the runtime is not in running state.
    ///
    /// \note This function needs to be executed on a HPX-thread. It will fail
    ///         otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_local_worker_thread_num() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current OS-thread running in the current
    ///        thread pool the current HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the OS-thread on the
    /// current thread pool which executes the current HPX-thread.
    ///
    /// \param ec [in,out] this represents the error status on exit (obsolete,
    ///        ignored).
    ///
    /// \note The returned value is zero based and its maximum value is smaller
    ///       than the number of OS-threads executed on the current thread pool.
    ///       It will return -1 if the current thread is not a known thread or
    ///       if the runtime is not in running state.
    ///
    /// \note This function needs to be executed on a HPX-thread. It will fail
    ///         otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_local_worker_thread_num(
        error_code&) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current thread pool the current
    ///        HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the thread pool which
    /// executes the current HPX-thread.
    ///
    /// \note The returned value is zero based and its maximum value is smaller
    ///       than the number of thread pools started by the runtime. It will
    ///       return -1 if the current thread pool is not a known thread pool or
    ///       if the runtime is not in running state.
    ///
    /// \note This function needs to be executed on a HPX-thread. It will fail
    ///         otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_thread_pool_num() noexcept;

    ///////////////////////////////////////////////////////////////////////////
    /// \brief Return the number of the current thread pool the current
    ///        HPX-thread is executed with.
    ///
    /// This function returns the zero based index of the thread pool which
    /// executes the current HPX-thread.
    ///
    /// \param ec [in,out] this represents the error status on exit (obsolete,
    ///        ignored).
    ///
    /// \note The returned value is zero based and its maximum value is smaller
    ///       than the number of thread pools started by the runtime. It will
    ///       return -1 if the current thread pool is not a known thread pool or
    ///       if the runtime is not in running state.
    ///
    /// \note This function needs to be executed on a HPX-thread. It will fail
    ///         otherwise (it will return -1).
    HPX_CORE_EXPORT std::size_t get_thread_pool_num(error_code&) noexcept;
}    // namespace hpx

#include <hpx/config/warnings_suffix.hpp>
