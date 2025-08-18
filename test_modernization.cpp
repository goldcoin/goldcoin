#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include <algorithm>

// Mock boost headers for testing
namespace boost {
    namespace signals2 {
        template<typename T>
        struct last_value {
            using result_type = T;
        };
        
        template<typename Signature, typename Combiner = void>
        class signal;
        
        template<typename R, typename... Args>
        class signal<R(Args...), void> {
        public:
            void connect(std::function<R(Args...)> callback) {}
            void disconnect(std::function<R(Args...)> callback) {}
            R operator()(Args... args) { 
                if constexpr (std::is_void_v<R>) {
                    return;
                } else {
                    return R{};
                }
            }
        };
        
        template<typename R, typename... Args, typename Combiner>
        class signal<R(Args...), Combiner> {
        public:
            void connect(std::function<R(Args...)> callback) {}
            void disconnect(std::function<R(Args...)> callback) {}
            R operator()(Args... args) { return R{}; }
        };
    }
}

// Test our modernization code
namespace UISignalUtils {
    struct CallbackConnection {
        std::string signal_name;
        std::string source_location;
        std::atomic<bool> is_active{true};
        
        CallbackConnection(const std::string& name, const std::string& location = "") 
            : signal_name(name), source_location(location) {}
    };
    
    template<typename CallbackType>
    class AutoCallback {
        std::vector<CallbackType>& callback_vector;
        size_t callback_index;
        
    public:
        AutoCallback(std::vector<CallbackType>& vec, CallbackType callback) 
            : callback_vector(vec) {
            callback_vector.push_back(std::move(callback));
            callback_index = callback_vector.size() - 1;
        }
        
        ~AutoCallback() {
            if (callback_index < callback_vector.size()) {
                if constexpr (std::is_same_v<CallbackType, std::function<void()>>) {
                    callback_vector[callback_index] = [](){};
                }
            }
        }
        
        AutoCallback(const AutoCallback&) = delete;
        AutoCallback& operator=(const AutoCallback&) = delete;
        AutoCallback(AutoCallback&&) = default;
        AutoCallback& operator=(AutoCallback&&) = default;
    };
    
    struct SignalDiagnostics {
        static std::atomic<size_t> total_callbacks_registered;
        static std::atomic<size_t> total_signals_fired;
        static std::atomic<size_t> boost_signals_fired;
        static std::atomic<size_t> modern_callbacks_fired;
        
        static void IncrementCallbacksRegistered() { total_callbacks_registered++; }
        static void IncrementSignalsFired() { total_signals_fired++; }
        static void IncrementBoostSignalsFired() { boost_signals_fired++; }
        static void IncrementModernCallbacksFired() { modern_callbacks_fired++; }
        
        static size_t GetTotalCallbacksRegistered() { return total_callbacks_registered.load(); }
        static size_t GetTotalSignalsFired() { return total_signals_fired.load(); }
        static size_t GetBoostSignalsFired() { return boost_signals_fired.load(); }
        static size_t GetModernCallbacksFired() { return modern_callbacks_fired.load(); }
    };
    
    template<typename CallbackType>
    void CompactCallbackVector(std::vector<CallbackType>& callbacks) {
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(), 
                [](const CallbackType& cb) {
                    return false; // Conservative: don't remove for now
                }), 
            callbacks.end()
        );
    }
}

// Static member definitions
std::atomic<size_t> UISignalUtils::SignalDiagnostics::total_callbacks_registered{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::total_signals_fired{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::boost_signals_fired{0};
std::atomic<size_t> UISignalUtils::SignalDiagnostics::modern_callbacks_fired{0};

// Test the modernized signal interface
class TestUIInterface {
public:
    // Modern std::function callbacks
    std::vector<std::function<void(const std::string&)>> InitMessageCallbacks;
    std::vector<std::function<void(int)>> NotifyNumConnectionsChangedCallbacks;
    
    // Legacy boost signals
    boost::signals2::signal<void(const std::string&)> InitMessage;
    boost::signals2::signal<void(int)> NotifyNumConnectionsChanged;
    
    // Modern callback management
    void AddInitMessageCallback(std::function<void(const std::string&)> callback) {
        InitMessageCallbacks.push_back(callback);
        UISignalUtils::SignalDiagnostics::IncrementCallbacksRegistered();
    }
    
    void TriggerInitMessage(const std::string& message) {
        InitMessage(message);
        UISignalUtils::SignalDiagnostics::IncrementBoostSignalsFired();
        
        for (auto& callback : InitMessageCallbacks) {
            callback(message);
            UISignalUtils::SignalDiagnostics::IncrementModernCallbacksFired();
        }
        UISignalUtils::SignalDiagnostics::IncrementSignalsFired();
    }
    
    size_t GetTotalModernCallbackCount() const {
        return InitMessageCallbacks.size() + NotifyNumConnectionsChangedCallbacks.size();
    }
};

int main() {
    TestUIInterface ui;
    
    // Test modern callback registration
    ui.AddInitMessageCallback([](const std::string& msg) {
        // Modern callback handler
    });
    
    // Test signal triggering
    ui.TriggerInitMessage("Test message");
    
    // Test diagnostics
    auto total_callbacks = UISignalUtils::SignalDiagnostics::GetTotalCallbacksRegistered();
    auto total_signals = UISignalUtils::SignalDiagnostics::GetTotalSignalsFired();
    
    return 0;
}