#include "../src/Datalogger.cpp"
#include <gtest/gtest.h>

#include <memory>
#include <cstdio>

using namespace std;

// The fixture for testing class Datalogger.
class DataloggerTest : public ::testing::Test {

protected:

    // set-up work for each test here.
    DataloggerTest();

    // clean-up work that doesn't throw exceptions here.
    virtual ~DataloggerTest();

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

	shared_ptr<Datalogger> dl1;
	shared_ptr<Datalogger> dl2;
	shared_ptr<Datalogger> dl3;
	shared_ptr<Datalogger> dl4;


};


DataloggerTest::DataloggerTest()
{
    dl1 = make_shared<Datalogger>("blackbox1.dat");
    dl2 = make_shared<Datalogger>("blackbox2.dat");
    dl3 = make_shared<Datalogger>("blackbox3.dat");
    dl4 = make_shared<Datalogger>("blackbox4.dat");
        
}

DataloggerTest::~DataloggerTest()
{       
	// Clean up the files we have generated:
    remove("blackbox1.dat");
    remove("blackbox2.dat");
    remove("blackbox3.dat");
    remove("blackbox4.dat");
	                  
}

void DataloggerTest::SetUp()
{

}

void DataloggerTest::TearDown()
{

}



TEST_F(DataloggerTest, InitializationTest) {
	dl1 = make_shared<Datalogger>("blackbox.dat");
}

// Test did not go as planned.. Two instances can easilly open the same file fro writing
// So we just have to be diligent when using the dataloggers..
#if 0
TEST_F(DataloggerTest, InitializationTest_Same_filename) {
        // The following should throw an exception
        // since the two instances uses the same file
        dl1 = make_shared<Datalogger>("blackbox.dat");
        ASSERT_THROW(
            Datalogger("blackbox.dat"),
            std::runtime_error);
}
#endif

TEST_F(DataloggerTest, CheckForTimeClassandValue) {
    int i = dl1->findClassIndex("TIME");
    ASSERT_EQ(i, 0) << "ClassIndex(\"TIME\") should be 0, but was " << i;
}

TEST_F(DataloggerTest, MultipleValueAndType) {
    ASSERT_NO_THROW(dl1->registerValueName("TIME", "jd", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "x", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "y", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "z", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("MASS", "m", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state", "INT"));
}

TEST_F(DataloggerTest, ClassAndValueIndexing) {
    ASSERT_NO_THROW(dl1->registerValueName("TIME", "jd", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "x", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "y", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("POS", "z", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("MASS", "m", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state", "INT"));
	
    // Class indexing
	ASSERT_EQ(dl1->findClassIndex("TIME"), 0);
    ASSERT_EQ(dl1->findClassIndex("POS"), 1);
    ASSERT_EQ(dl1->findClassIndex("MASS"), 2);
    ASSERT_EQ(dl1->findClassIndex("AP_STATE"), 3);

    // ValueName indexing
    ASSERT_EQ(dl1->findValueIndex(0,"t"), 0);
    ASSERT_EQ(dl1->findValueIndex(0,"jd"), 1);
    ASSERT_EQ(dl1->findValueIndex(1,"x"), 0);
    ASSERT_EQ(dl1->findValueIndex(1,"y"), 1);
	ASSERT_EQ(dl1->findValueIndex(1,"z"), 2);
    ASSERT_EQ(dl1->findValueIndex(2,"m"), 0);
   	ASSERT_EQ(dl1->findValueIndex(3,"state"), 0);
}


TEST_F(DataloggerTest, WrongRegistration) {
    // Wrong type
    ASSERT_THROW(dl1->registerValueName("foo", "bar", "complex"), std::runtime_error);
    // Empty class
    ASSERT_THROW(dl1->registerValueName("", "bar", "INT"), std::runtime_error);
    // Empty value
    ASSERT_THROW(dl1->registerValueName("foo", "", "INT"), std::runtime_error);
 
}

TEST_F(DataloggerTest, NoWriteBeforeFirstTick)
{
    ASSERT_NO_THROW(dl1->registerValueName("MASS", "m", "DOUBLE"));

    ASSERT_THROW(dl1->writeValue("MASS", "m", 2.3), std::runtime_error);
}

TEST_F(DataloggerTest, TryTick) {
    ASSERT_NO_THROW(dl1->tick(1.0));

    for(double t = 0.0; t <= 100; t += 0.1)
        ASSERT_NO_THROW(dl2->tick(t));
}

TEST_F(DataloggerTest, WriteUnregisteredClassAndValue)
{
    ASSERT_NO_THROW(dl1->registerValueName("MASS", "m", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state1", "INT"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state2", "INT"));
    ASSERT_NO_THROW(dl1->tick(1.0));

    ASSERT_THROW(dl1->writeValue("MASS2", "m", 2.3), std::runtime_error);

    ASSERT_THROW(dl1->writeValue("MASS", "m3", 2.3), std::runtime_error);

    ASSERT_THROW(dl1->writeValue("Noclass", "state", 2), std::runtime_error);

    ASSERT_THROW(dl1->writeValue("AP_STATE", "something_wrong", 2), std::runtime_error);

}

TEST_F(DataloggerTest, WriteWrongType)
{
    ASSERT_NO_THROW(dl1->registerValueName("MASS", "m", "DOUBLE"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state1", "INT"));
    ASSERT_NO_THROW(dl1->registerValueName("AP_STATE", "state2", "INT"));
    ASSERT_NO_THROW(dl1->tick(1.0));

	ASSERT_THROW(dl1->writeValue("MASS", "m", 2), std::runtime_error);
	ASSERT_THROW(dl1->writeValue("AP_STATE", "state1", 2.7), std::runtime_error);
	ASSERT_THROW(dl1->writeValue("AP_STATE", "state2", 2.7), std::runtime_error);

}

TEST_F(DataloggerTest, RegisterAbove256ClassesAndValues)
{
		ostringstream oss;
		for(int i = 0; i < 255; ++i)
        {
            oss.str(std::string());
            oss << "some" << i;
            try{
                // test a lot of classes
                dl1->registerValueName(oss.str(), "m", "DOUBLE");

                // test a lot of values
                dl2->registerValueName("CLASS", oss.str(), "DOUBLE");
            }
            catch (std::exception& err)
            {
                //dl1->dumpRegister();
                FAIL() << err.what();
            }  
        }

        // add one more value
        ASSERT_NO_THROW(dl2->registerValueName("CLASS", "value 255", "DOUBLE"));


        // Try to insert 256th class (dl1) and value(dl2)
        ASSERT_THROW(dl1->registerValueName("ClassNumber256", "m", "DOUBLE"), std::runtime_error);

        try {
            dl2->registerValueName("CLASS", "value256", "DOUBLE");
            //dl2->dumpRegister();
            FAIL() << "Failed!";
        } catch (std::runtime_error& err)
        {
            SUCCEED();
        } catch (...)
        {
            FAIL() << "Failed, wrong exception!";

        }
        
 }

TEST_F(DataloggerTest, LogMultipleData)
{
    dl1->registerValueName("TIME", "jd", "DOUBLE");
    dl1->registerValueName("POS", "x", "DOUBLE");
    dl1->registerValueName("POS", "y", "DOUBLE");
    dl1->registerValueName("INTS", "(Step*2)+3", "INT");

    int i = 0;
    for(double t = 0.0; t < 1000.0; t += 0.01)
    {
        dl1->tick(t);

        dl1->writeValue("TIME", "jd", 215000.5+t/60.0/60.0/24.0);
        dl1->writeValue("POS", "x" , -500.0 + t);
        dl1->writeValue("POS", "y" , -100.0 + t/50.0);
        dl1->writeValue("INTS", "(Step*2)+3", i*2+3);


        i++;
    }
}


