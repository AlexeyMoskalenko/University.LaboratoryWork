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
    //Arrange
    MockInheritorIKeypad Keypad;

    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);
    //Prepare mocks
    EXPECT_CALL(Keypad, wait())
            .Times(1);
    //Act
    LockerInstance.wait();
}

TEST(LockerTest, Test2){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Latch, getDoorStatus())
            .Times(1)
            .WillOnce(Return(DoorStatus::CLOSE));

    //Assert
    ASSERT_FALSE(LockerInstance.isDoorOpen());
}

TEST(LockerTest, Test3){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Latch, getDoorStatus())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    //Assert
    ASSERT_TRUE(LockerInstance.isDoorOpen());
}

TEST(LockerTest, Test4){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks
    EXPECT_CALL(Latch, open())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    //Assert
    ASSERT_EQ(LockerInstance.unlockDoor(), DoorStatus::OPEN);
}

TEST(LockerTest, Test5){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks
    EXPECT_CALL(Latch, open())
            .Times(1)
            .WillOnce(Return(DoorStatus::OPEN));

    //Assert
    ASSERT_EQ(LockerInstance.lockDoor(), DoorStatus::CLOSE);
}

TEST(LockerTest, Test6){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Keypad, isActive())
            .Times(1)
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(1)
            .WillOnce(Return(true));

    //Assert
    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::OK);
}

TEST(LockerTest, Test7){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(nullptr, &Latch);

    //Prepare mocks

    EXPECT_CALL(Keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    //Assert
    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(LockerTest, Test8){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Keypad, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(true));

    EXPECT_CALL(Latch, isActive())
            .Times(AtLeast(0))
            .WillOnce(Return(false));

    //Assert
    ASSERT_EQ(LockerInstance.hardWareCheck(), HardWareStatus::ERROR);
}

TEST(LockerTest, Test9){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Keypad, requestPassword)
            .Times(1)
            .WillOnce(Return(
                                []{
                                    PasswordResponse Respone{ PasswordResponse::Status::OK, "0000" };
                                    return Respone;
                                }()
                            ));

    //Assert
    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

TEST(LockerTest, Test10){
    //Arrange
    MockInheritorIKeypad Keypad;
    MockInheritorILatch Latch;
    LockController LockerInstance(&Keypad, &Latch);

    //Prepare mocks

    EXPECT_CALL(Keypad, requestPassword)
            .Times(1)
            .WillOnce(Return(
                                []{
                                    PasswordResponse Respone{ PasswordResponse::Status::OK, "6996"};
                                    return Respone;
                                }()
                            ));

    //Assert
    ASSERT_FALSE(LockerInstance.isCorrectPassword());
};

TEST(LockerTest, Test11){
    //Arrange
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

    //Prepare mocks
    EXPECT_CALL(Keypad, requestPassword)
            .Times(3)
            .WillOnce(Return(Password(passDict[0])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[1])));

    //Act
    LockerInstance.resetPassword();

    //Assert
    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

TEST(LockerTest, Test12){
    //Arrange
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

    //Prepare mocks
    EXPECT_CALL(Keypad, requestPassword)
            .Times(5)
            .WillOnce(Return(Password(passDict[0])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[1])))
            .WillOnce(Return(Password(passDict[2])))
            .WillOnce(Return(Password(passDict[2])));

    //Act
    LockerInstance.resetPassword();
    LockerInstance.resetPassword();
    //Assert
    ASSERT_TRUE(LockerInstance.isCorrectPassword());
}

#endif // TST_TESTSUIT_H
