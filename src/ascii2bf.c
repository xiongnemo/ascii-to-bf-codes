#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define INPUT_BUFFER_LENGTH 256
#define OUTPUT_BUFFER_LENGTH 4096
#define TEMP_BUFFER_LENGTH 64

// just a wrapper, as char_before can be assumed as 0
void deal_with_first_char(char first_char, char *head);

// calc how to use an addition and a multiplication to transform char_before into char_after.
void deal_with_successive_chars(char char_before, char char_after, char *head);

// transfer before into after, fit into a +/- b * (c / d) with a modular of 256
// write result to result[]
// assume it can be fitted, or it will prompt no solution.
// known issue(s): sometimes the cacluation give wrong result, as in actual brainfxxk implementation, 
//                 there maybe early zeros when doing modulo operation.
// result: 
// result[0]: 1 for postive, -1 for negative, 0 to neglect itself and result[1]
// result[1]: quantity to add or subtract
// result[2]: numerator of multiplication
// result[3]: denominator of multiplication
void fit_into_addition_and_multiplication(char before, char after, int *result);

// generate brainfxxk codes for multiply current cell's value by (numerator / denominator)
// like "[--->+++++<]" do a multiplication of 5 / 3 to current value and store it to next cell
// parameters: numerator, denominator, char* data for data output
// output: "[" + denominator * "-" + ">" + numerator * "+" + "<]"
void generate_bf_codes_for_multiplication(int numerator, int denominator, char *data);

// generate brainfxxk codes for adding current cell's value by quantity with signing
// like "++++" do an addition of 4 to current cell, while "-----" do a subtraction of 5
// parameters: bool positive, quantity, char* data for data output
// output: if postive, output quantity * "+"; if not then output quantity * "-"
void generate_bf_codes_for_addition(bool positive, int quantity, char *data);

int main(void)
{
    char input[INPUT_BUFFER_LENGTH];
    char output[OUTPUT_BUFFER_LENGTH] = "";
    scanf("%s", input);
    int input_length = strlen(input);
    // input ranges from 32 (whitespace) to 126 ("~")
    deal_with_first_char(input[0], output);
    int output_length = strlen(output);
    for (int current_pos = 1; current_pos < input_length; current_pos++)
    {
        deal_with_successive_chars(input[current_pos - 1], input[current_pos], &output[output_length]);
        output_length = strlen(output);
    }
    printf("%s", output);
    return 0;
}

void deal_with_first_char(char first_char, char *head)
{
    deal_with_successive_chars(0, first_char, head);
}

void deal_with_successive_chars(char char_before, char char_after, char *head)
{
    int current_length = strlen(head);
    if (char_before == char_after)
    {
        head[current_length] = '.';
        head[current_length + 1] = '\0';
        return;
    }
    if (char_before - char_after < 0 && char_before - char_after > -10)
    {
        generate_bf_codes_for_addition(true, char_after - char_before, &head[current_length]);
        current_length = strlen(head);
        head[current_length] = '.';
        head[current_length + 1] = '\0';
        return;
    }
    else
    {
        if (char_before - char_after > 0 && char_before - char_after < 10)
        {
            generate_bf_codes_for_addition(false, char_before - char_after, &head[current_length]);
            current_length = strlen(head);
            head[current_length] = '.';
            head[current_length + 1] = '\0';
            return;
        }
        else
        {
            // try to fix into an addition and a multiplication
            int result[4] = {114, 514, 1919, 810};
            fit_into_addition_and_multiplication(char_before, char_after, result);
            if (result[0])
            {
                bool is_postive = (result[0] == 1);
                generate_bf_codes_for_addition(is_postive, result[1], &head[current_length]);
            }
            current_length = strlen(head);
            generate_bf_codes_for_multiplication(result[2], result[3], &head[current_length]);
            current_length = strlen(head);
            head[current_length] = '>';
            head[current_length + 1] = '.';
            head[current_length + 2] = '\0';
            return;
        }
    }
}

void fit_into_addition_and_multiplication(char before, char after, int *result)
{
    if (before % after == 0 && before != 0)
    {
        result[0] = 0;
        result[1] = 0;
        result[2] = 1;
        result[3] = (before / after);
        return;
    }
    else
    {
        if (before != 0 && after % before == 0)
        {
            result[0] = 0;
            result[1] = 0;
            result[2] = (after / before);
            result[3] = 1;
            return;
        }
        else
        {
            // bruuuuuuute!
            for (int i = -10; i < 11; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    for (int n = 1; n < 11; n++)
                    {
                        for (int d = 1; d < 11; d++)
                        {
                            if ((before + i + 256 * j) * n % d == 0)
                            {
                                if ((before + i + 256 * j) * n / d == after)
                                {
                                    if (i == 0)
                                    {
                                        result[0] = 0;
                                        result[1] = 0;
                                    }
                                    else
                                    {
                                        result[0] = (i > 0 ? 1 : -1);
                                        result[1] = result[0] * i;
                                    }
                                    result[2] = n;
                                    result[3] = d;
                                    // printf("%d %d %d %d before: %d after: %d\n", result[0], result[1], result[2], result[3], before, after);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            fprintf(stderr, "Currently no solution\n");
            exit(1);
        }
    }
}

void generate_bf_codes_for_multiplication(int numerator, int denominator, char *data)
{
    int data_proposed_length = 1 + denominator + 1 + numerator + 2;
    if (data_proposed_length > TEMP_BUFFER_LENGTH)
    {
        fprintf(stderr, "Generate brainfxxk codes for multiplication failed: numerator = %d, denominator = %d\n", numerator, denominator);
        fprintf(stderr, "Reason: proposed length %d is larger than TEMP_BUFFER_LENGTH %d.\n", data_proposed_length, TEMP_BUFFER_LENGTH);
        fprintf(stderr, "Parameters: numerator = %d, denominator = %d\n", numerator, denominator);
        exit(-1);
    }
    data[0] = '[';
    int current_pos = 1;
    int denominator_end_pos = 1 + denominator;
    for (current_pos = 1; current_pos < denominator_end_pos; current_pos++)
    {
        data[current_pos] = '-';
    }
    data[current_pos] = '>';
    current_pos++;
    int numerator_end_pos = denominator_end_pos + 1 + numerator;
    for (; current_pos < numerator_end_pos; current_pos++)
    {
        data[current_pos] = '+';
    }
    data[current_pos] = '<';
    current_pos++;
    data[current_pos] = ']';
    current_pos++;
    data[current_pos] = '\0';
}

void generate_bf_codes_for_addition(bool positive, int quantity, char *data)
{
    int data_proposed_length = quantity;
    if (data_proposed_length > TEMP_BUFFER_LENGTH)
    {
        fprintf(stderr, "Generate brainfxxk codes for addition failed: quantity = %d.\n", quantity);
        fprintf(stderr, "Reason: proposed length %d is larger than TEMP_BUFFER_LENGTH %d.\n", data_proposed_length, TEMP_BUFFER_LENGTH);
        exit(-2);
    }
    char char_to_fill = (positive ? '+' : '-');
    int current_pos = 0;
    for (current_pos = 0; current_pos < data_proposed_length; current_pos++)
    {
        data[current_pos] = char_to_fill;
    }
    data[current_pos] = '\0';
}