#ifndef TST_TESTSUIT_H
#define TST_TESTSUIT_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "lockcontroller.h"

using namespace testing;

class MockInheritorIKeypad : public IKeypad{
public:
    MOCK_METHOD( bool, isActive, (), (override) );
    MOCK_METHOD( void, wait, (), (override) );
    MOCK_METHOD( PasswordResponse, requestPassword, (), (override) );
};

class MockInheritorILatch : public ILatch{
public:
    MOCK_METHOD( bool, isActive, (), (override) );
    MOCK_METHOD( DoorStatus, open, (), (override) );
    MOCK_METHOD( DoorStatus, close, (), (override) );
    MOCK_METHOD( DoorStatus, getDoorStatus, (), (override) );
};

TEST(LockerTest, Test1){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);
    
    EXPECT_CALL(Keypad, wait())
            .Times(1);
			
    LockerInstance.wait();
}

TEST(LockerTest, Test2){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);
	
    EXPECT_CALL(Latch, getDoorStatus())
            .Times(1)
            .WillOnce(Return(DoorStatus::CLOSE));

    ASSERT_FALSE(LockerInstance.isDoorOpen());
}

TEST(LockerTest, Test3){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Latch, getDoorStatus())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    ASSERT_TRUE(LockerInstance.isDoorOpen());
}

TEST(LockerTest, Test4){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Latch, open())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    ASSERT_EQ(LockerInstance.unlockDoor(), DoorStatus::OPEN);
}

TEST(LockerTest, Test5){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Latch, open())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    ASSERT_EQ(LockerInstance.lockDoor(), DoorStatus::CLOSE);
}

TEST(LockerTest, Test6){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Keypad, isActive())
            .Times(1)
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(1)
            .WillOnce(Return(true));

    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::OK);
}

TEST(LockerTest, Test7){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(nullptr, &Latch);

    EXPECT_CALL(Keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(LockerTest, Test8){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(false));

    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(LockerTest, Test9){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Keypad, requestPassword)
            .Times(1)
            .WillOnce(Return(
                                []{
                                    PasswordResponse Respone{ PasswordResponse::Status::OK, "0000" };
                                    return Respone;
                                }()
                            ));

    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

TEST(LockerTest, Test10){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    EXPECT_CALL(Keypad, requestPassword)
            .Times(1)
            .WillOnce(Return(
                                []{
                                    PasswordResponse Respone{ PasswordResponse::Status::OK, "6996"};
                                    return Respone;
                                }()
                            ));

    ASSERT_FALSE(LockerInstance.isCorrectPassword());
};

TEST(LockerTest, Test11){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    std::string passDict[3]{"0000", "6996"};

    auto Password = [](std::string password){
        PasswordResponse PasswordStruct{
            PasswordResponse::Status::OK,
            password
        };
        return PasswordStruct;
    };

    EXPECT_CALL(Keypad, requestPassword)
            .Times(3)
            .WillOnce(Return(Password(passDict[0])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[1])));

    LockerInstance.resetPassword();

    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

TEST(LockerTest, Test12){
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    std::string passDict[3]{"0000","9876","6996"};

    auto Password = [](std::string password){
        PasswordResponse PasswordStruct{
            PasswordResponse::Status::OK,
            password
        };
        return PasswordStruct;
    };

    EXPECT_CALL(Keypad, requestPassword)
            .Times(5)
            .WillOnce(Return(Password(passDict[0])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[2])))
            .WillOnce(Return(Password(passDict[2])));

    LockerInstance.resetPassword();
    LockerInstance.resetPassword();

    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

#endif // TST_TESTSUIT_H
