
#include "addtime.hpp"

namespace IO
{
    /** This function is a replacement for std::endl. It prints a newline 
     *  character to the specified output stream, but it elides the implicit 
     *  call to std::fflush(), thereby foregoing the output stream flush on 
     *  each call.
     * 
     */
    template <typename T, typename CharT = std::char_traits<T>>
    std::basic_ostream<T, CharT>&
    endl(std::basic_ostream<T, CharT>& outputStream)
    {
        return outputStream << outputStream.widen('\n');
    }
} // namespace IO

class Time
{
    uint64_t s;
    uint64_t m;
    uint64_t h;

    void reduce()
    {
        while (s >= 60) {
            ++m;
            s -= 60;
        }

        while (m >= 60) {
            ++h;
            m -= 60;
        }
    }

    void initializeFromString(const char* str)
    {
        uint64_t seconds_field = 0;
        uint64_t minutes_field = 0;
        uint64_t __hours_field = 0;

        bool flag_minutes = false;
        bool flag_hours   = false;

        for (int i = strlen(str) - 1; i >= 0; --i) {
            if (str[i] == ':') {
                if (flag_minutes) {
                    flag_hours = true;

                    continue;
                }

                flag_minutes = true;
                
                continue;
            }

            const uint64_t val = str[i] - '0';

            if ((!flag_hours) && (!flag_minutes)) {
                // We're still in the seconds field
                s += pow(10, seconds_field++) * val;
            } else if (!flag_hours) {
                // We're in the minutes field
                m += pow(10, minutes_field++) * val;
            } else {
                // We're in the hours field
                h += pow(10, __hours_field++) * val;
            }
        }

        reduce();
    }

public:
    Time()
        : s { 0 }, m { 0 }, h { 0 }
    {
        //
    }

    Time(uint64_t seconds)
        : s { seconds }, m { 0 }, h { 0 }
    {
        reduce();
    }

    Time(uint64_t hours, uint64_t minutes, uint64_t seconds)
        : s { seconds }, m { minutes }, h { hours }
    {
        reduce();
    }

    Time(const char* str)
        : s { 0 }, m { 0 }, h { 0 }
    {
        initializeFromString(str);
    }

    Time(const std::string& str)
        : s { 0 }, m { 0 }, h { 0 }
    {
        initializeFromString(str.c_str());
    }

    uint64_t seconds() const noexcept { return s; }

    uint64_t minutes() const noexcept { return m; }

    uint64_t   hours() const noexcept { return h; }

    Time& operator += (const Time& other)
    {
        s += other.seconds();
        m += other.minutes();
        h += other.hours();
        
        reduce();

        return *this;
    }

    Time& Add(const char* other)
    {
        *this += Time(other);

        return *this;
    }

    Time& Add(const Time& other)
    {
        *this += other;

        return *this;
    }

    Time operator + (const Time& other)
    {
        const uint64_t sec = s + other.seconds();
        const uint64_t min = m + other.minutes();
        const uint64_t hrs = h + other.hours();

        return Time(hrs, min, sec);
    }

    friend std::ostream& operator << (std::ostream& outputStream, const Time& time)
    {
        return outputStream
            << std::setfill(' ') << std::setw(8) << time.hours()   << ":"
            << std::setfill('0') << std::setw(2) << time.minutes() << ":"
            << std::setfill('0') << std::setw(2) << time.seconds();
    }
};

template <typename... T>
Time AddTimes(T... times)
{
    return (times + ...);
}

namespace {

// class TestFixtureTime : testing::Test
// {
//     Time t;

// public:
//     void SetUp() override
//     {
//         //
//     }
// };

TEST(TimeConstructor, DefaultConstructor)
{
    Time t;

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 0);
    EXPECT_EQ(t.hours(),   0);
}

TEST(TimeConstructor, SixtySeconds)
{
    Time t(60);

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 1);
    EXPECT_EQ(t.hours(),   0);
}

TEST(TimeConstructor, SixtyMinutes)
{
    Time t(0, 60, 0);

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 0);
    EXPECT_EQ(t.hours(),   1);
}

TEST(TimeConstructor, SixtyOneSeconds)
{
    Time t1(61);

    EXPECT_EQ(t1.seconds(), 1);
    EXPECT_EQ(t1.minutes(), 1);
    EXPECT_EQ(t1.hours(),   0);

    Time t2(0, 0, 61);

    EXPECT_EQ(t2.seconds(), 1);
    EXPECT_EQ(t2.minutes(), 1);
    EXPECT_EQ(t2.hours(),   0);
}

TEST(TimeConstructor, SixtyOneMinutes)
{
    Time t(0, 61, 0);

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 1);
    EXPECT_EQ(t.hours(),   1);
}

TEST(TimeStringConstructor, EmptyString)
{
    Time t("");

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 0);
    EXPECT_EQ(t.hours(),   0);
}

TEST(TimeStringConstructor, ZeroZeroZero)
{
    Time t("0:0:0");

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 0);
    EXPECT_EQ(t.hours(),   0);
}

TEST(TimeStringConstructor, DoubleZeroZeroZero)
{
    Time t("00:00:00");

    EXPECT_EQ(t.seconds(), 0);
    EXPECT_EQ(t.minutes(), 0);
    EXPECT_EQ(t.hours(),   0);
}

TEST(TimeStringConstructor, SixtyOneSixtyOneSixtyOne)
{
    Time t("61:61:61");

    EXPECT_EQ(t.seconds(), 1);
    EXPECT_EQ(t.minutes(), 2);
    EXPECT_EQ(t.hours(),   62);
}

TEST(TimeAddToSelf, ZeroPlusZero)
{
    Time t1;
    Time t2;

    EXPECT_EQ(t1.seconds(), 0);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 0);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);

    t1 += t2;

    EXPECT_EQ(t1.seconds(), 0);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 0);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);
}

TEST(TimeAddToSelf, OneSecondPlusOneSecond)
{
    Time t1(1);
    Time t2(1);

    EXPECT_EQ(t1.seconds(), 1);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 1);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);

    t1 += t2;

    EXPECT_EQ(t1.seconds(), 2);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 1);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);
}

TEST(TimeAdd, ZeroPlusZero)
{
    Time t1;
    Time t2;

    EXPECT_EQ(t1.seconds(), 0);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 0);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);

    Time t = t1 + t2;

    EXPECT_EQ(t.seconds(),  0);
    EXPECT_EQ(t.minutes(),  0);
    EXPECT_EQ(t.hours(),    0);

    EXPECT_EQ(t1.seconds(), 0);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 0);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);
}

TEST(TimeAdd, OneSecondPlusOneSecond)
{
    Time t1(0, 0, 1);
    Time t2(0, 0, 1);

    EXPECT_EQ(t1.seconds(), 1);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 1);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);

    Time t = t1 + t2;

    EXPECT_EQ(t.seconds(),  2);
    EXPECT_EQ(t.minutes(),  0);
    EXPECT_EQ(t.hours(),    0);

    EXPECT_EQ(t1.seconds(), 1);
    EXPECT_EQ(t1.minutes(), 0);
    EXPECT_EQ(t1.hours(),   0);

    EXPECT_EQ(t2.seconds(), 1);
    EXPECT_EQ(t2.minutes(), 0);
    EXPECT_EQ(t2.hours(),   0);
}

TEST(TimeAddFunction, SimpleChain)
{
    Time time;

    EXPECT_EQ(time.seconds(), 0);
    EXPECT_EQ(time.minutes(), 0);
    EXPECT_EQ(time.hours(),   0);

    time.Add("0:00:01")
        .Add("0:00:02")
        .Add("0:00:03")
        .Add("0:00:04")
        .Add("0:00:05");
    
    EXPECT_EQ(time.seconds(), 15);
    EXPECT_EQ(time.minutes(),  0);
    EXPECT_EQ(time.hours(),    0);
}

TEST(TimeAddFoldExpression, SimpleFold)
{
    Time aggregate =
        AddTimes(
            Time("0:00:01"),
            Time("0:00:02"),
            Time("0:00:03"),
            Time("0:00:04"),
            Time("0:00:05"),
            Time("0:00:06"),
            Time("0:00:07"),
            Time("0:00:08"),
            Time("0:00:09"),
            Time("0:00:10"),
            Time("0:00:11"),
            Time("0:00:12"),
            Time("0:00:13"),
            Time("0:00:14"),
            Time("0:00:15"),
            Time("0:00:16"),
            Time("0:00:17"),
            Time("0:00:18"),
            Time("0:00:19"),
            Time("0:00:20")
    );

    EXPECT_EQ(aggregate.seconds(), 30);
    EXPECT_EQ(aggregate.minutes(),  3);
    EXPECT_EQ(aggregate.hours(),    0);
}

} // Time tests

int main(int argc, char *argv[])
{
    bool option_verbose = false;

    std::vector<std::string> values;

    Options::options_description generic("  Generic Options");
    generic.add_options()
        ("help", "Display this help menu and exit.")
        ("version", "Display program version information and exit.")
    ;

    Options::options_description exec("  Execution Options");
    exec.add_options()
        ("verbose", "Display detailed information during program execution")
    ;

    Options::options_description config("  Configuration Options");
    config.add_options()
        ("time-value", Options::value<std::vector<std::string>>(&values)->composing(), "Time value(s)")
    ;

    Options::options_description desc("Program Options");
    desc
        .add(generic)
        .add(exec)
        .add(config);
    
    Options::positional_options_description pos;
    pos.add("time-value", -1);

    Options::variables_map vm;
    Options::store(
        Options::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos)
            .run(),
        vm);
    Options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << IO::endl;

        return EXIT_SUCCESS;
    }

    if (vm.count("version")) {
        std::cout << "<Program Version Info>" << IO::endl;

        return EXIT_SUCCESS;
    }

    if (!vm.count("time-value")) {
        std::cerr << "No times specified." << IO::endl << IO::endl;
        std::cout << desc << IO::endl;
        std::exit(EXIT_FAILURE);
    }

    if (vm.count("verbose")) {
        option_verbose = true;
    }

    #ifndef NDEBUG
    testing::InitGoogleTest(&argc, argv);
    #endif

    Time aggregate;

    for (const auto& value : values) {
        const Time time(value);

        aggregate.Add(time);
        
        if (option_verbose) {
            std::cout << time << IO::endl;
        }
    }

    if (option_verbose) {
        std::cout << " ------------------ " << IO::endl;
    }

    std::cout << aggregate << IO::endl << IO::endl;

    return EXIT_CODE;
}
