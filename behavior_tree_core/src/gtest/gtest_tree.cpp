#include <gtest/gtest.h>
#include "behavior_tree.h"

struct SimpleSequenceTest : testing::Test
{
    BT:: SequenceNode* root;
    BT::ActionTestNode* action;
     BT::ConditionTestNode* condition;
    SimpleSequenceTest()
    {
        action = new BT::ActionTestNode("action");
        condition = new BT::ConditionTestNode("condition");

        root = new BT::SequenceNode("seq1");

        root->AddChild(condition);
        root->AddChild(action);
    }


};


struct ComplexSequenceTest : testing::Test
{
    BT:: SequenceNode* root;
    BT::ActionTestNode* action_1;
    BT::ConditionTestNode* condition_1;
    BT::ConditionTestNode* condition_2;

     BT:: SequenceNode* seq_conditions;
     BT:: SequenceNode* seq_actions;

    ComplexSequenceTest()
    {
        action_1 = new BT::ActionTestNode("action 1");
        condition_1 = new BT::ConditionTestNode("condition 1");
        condition_2 = new BT::ConditionTestNode("condition 2");
        seq_conditions = new BT::SequenceNode("sequence_conditions");

        seq_conditions->AddChild(condition_1);
        seq_conditions->AddChild(condition_2);

        root = new BT::SequenceNode("root");
        root->AddChild(seq_conditions);
        root->AddChild(action_1);
    }
};



struct SimpleFallbackTest : testing::Test
{
    BT:: FallbackNode* root;
    BT::ActionTestNode* action;
     BT::ConditionTestNode* condition;
    SimpleFallbackTest()
    {
        action = new BT::ActionTestNode("action");
        condition = new BT::ConditionTestNode("condition");

        root = new BT::FallbackNode("seq1");

        root->AddChild(condition);
        root->AddChild(action);
    }


};


struct ComplexFallbackTest : testing::Test
{
    BT:: FallbackNode* root;
    BT::ActionTestNode* action_1;
    BT::ConditionTestNode* condition_1;
    BT::ConditionTestNode* condition_2;

     BT:: FallbackNode* sel_conditions;

    ComplexFallbackTest()
    {
        action_1 = new BT::ActionTestNode("action 1");
        condition_1 = new BT::ConditionTestNode("condition 1");
        condition_2 = new BT::ConditionTestNode("condition 2");
        sel_conditions = new BT::FallbackNode("fallback_conditions");

        sel_conditions->AddChild(condition_1);
        sel_conditions->AddChild(condition_2);

        root = new BT::FallbackNode("root");
        root->AddChild(sel_conditions);
        root->AddChild(action_1);
    }
};




struct BehaviorTreeTest : testing::Test
{
    BT:: SequenceNode* root;
    BT::ActionTestNode* action_1;
    BT::ConditionTestNode* condition_1;
    BT::ConditionTestNode* condition_2;

     BT:: FallbackNode* sel_conditions;

    BehaviorTreeTest()
    {
        action_1 = new BT::ActionTestNode("action 1");
        condition_1 = new BT::ConditionTestNode("condition 1");
        condition_2 = new BT::ConditionTestNode("condition 2");
        sel_conditions = new BT::FallbackNode("fallback_conditions");

        sel_conditions->AddChild(condition_1);
        sel_conditions->AddChild(condition_2);

        root = new BT::SequenceNode("root");
        root->AddChild(sel_conditions);
        root->AddChild(action_1);
    }
};


TEST_F(SimpleSequenceTest, ConditionTrue) {

    std::cout << "Ticking the root node !" << std::endl << std::endl;
    // Ticking the root node
    BT::ReturnStatus state = root->Tick();

    ASSERT_EQ(BT::RUNNING, action->get_status());
    ASSERT_EQ(BT::RUNNING, state);
}


TEST_F(SimpleSequenceTest, ConditionTurnToFalse) {

    BT::ReturnStatus state = root->Tick();


    condition->set_boolean_value(false);

    state = root->Tick();
    ASSERT_EQ(BT::FAILURE, state);
    ASSERT_EQ(BT::HALTED, action->get_status());
    root->Halt();

}


TEST_F(ComplexSequenceTest, ComplexSequenceConditionsTrue) {

        BT::ReturnStatus state = root->Tick();
        //    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

        ASSERT_EQ(BT::RUNNING, action_1->get_status());
        ASSERT_EQ(BT::RUNNING, state);

}




TEST_F(ComplexSequenceTest, ComplexSequenceConditions1ToFalse) {

        BT::ReturnStatus state = root->Tick();

        condition_1->set_boolean_value(false);

        state = root->Tick();

        ASSERT_EQ(BT::FAILURE, state);
        ASSERT_EQ(BT::HALTED, action_1->get_status());
}

TEST_F(ComplexSequenceTest, ComplexSequenceConditions2ToFalse) {

        BT::ReturnStatus state = root->Tick();

        condition_2->set_boolean_value(false);

        state = root->Tick();

        ASSERT_EQ(BT::FAILURE, state);
        ASSERT_EQ(BT::HALTED, action_1->get_status());
}



TEST_F(SimpleFallbackTest, ConditionTrue) {

    std::cout << "Ticking the root node !" << std::endl << std::endl;
    // Ticking the root node
    condition->set_boolean_value(true);
    BT::ReturnStatus state = root->Tick();

    ASSERT_EQ(BT::IDLE, action->get_status());
    ASSERT_EQ(BT::SUCCESS, state);
}


TEST_F(SimpleFallbackTest, ConditionToFalse) {

    condition->set_boolean_value(false);

    BT::ReturnStatus state = root->Tick();
    condition->set_boolean_value(true);


    state = root->Tick();

    ASSERT_EQ(BT::SUCCESS, state);
    ASSERT_EQ(BT::HALTED, action->get_status());
}


TEST_F(ComplexFallbackTest, Condition1ToTrue) {

       condition_1->set_boolean_value(false);
       condition_2->set_boolean_value(false);

        BT::ReturnStatus state = root->Tick();

        condition_1->set_boolean_value(true);

        state = root->Tick();

        ASSERT_EQ(BT::SUCCESS, state);
        ASSERT_EQ(BT::HALTED, action_1->get_status());
}

TEST_F(ComplexFallbackTest, Condition2ToTrue) {

    condition_1->set_boolean_value(false);
    condition_2->set_boolean_value(false);

     BT::ReturnStatus state = root->Tick();

     condition_2->set_boolean_value(true);

     state = root->Tick();

     ASSERT_EQ(BT::SUCCESS, state);
     ASSERT_EQ(BT::HALTED, action_1->get_status());
}



TEST_F(BehaviorTreeTest, Condition1ToFalseCondition2True) {

    condition_1->set_boolean_value(false);
    condition_2->set_boolean_value(true);

     BT::ReturnStatus state = root->Tick();

     ASSERT_EQ(BT::RUNNING, state);
     ASSERT_EQ(BT::RUNNING, action_1->get_status());
}

TEST_F(BehaviorTreeTest, Condition2ToFalseCondition1True) {

    condition_2->set_boolean_value(false);
    condition_1->set_boolean_value(true);

     BT::ReturnStatus state = root->Tick();

     ASSERT_EQ(BT::RUNNING, state);
     ASSERT_EQ(BT::RUNNING, action_1->get_status());
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "BehaviorTree");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

