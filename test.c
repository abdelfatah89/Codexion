/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   test.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: alaktaou <alaktaou@student.1337.ma>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/15 09:40:41 by alaktaou         #+#    #+#              */
/*   Updated: 2026/06/15 09:40:41 by alaktaou        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

// Define a sample structure
typedef struct s
{
	int		id;
	float	score;
}	Student;

int	main(void)
{
	int     n;
	Student *students;

    n = 5
    students = malloc(n *sizeof(*students));
	if (students == NULL)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		return (1);
	}
	// 3. Initialize and populate the array using normal array syntax
	for (int i = 0; i < n; i++)
	{
		students[i].id = 100 + i;
		students[i].score = 90.5f + i;
	}
	// 4. Access and print the data
	for (int i = 0; i < n; i++)
	{
		printf("Student %d: ID = %d, Score = %.1f\n", i, students[i].id, students[i].score);
	}
	// 5. Free the allocated memory when finished
	free(students);
	students = NULL;
	// Prevent dangling pointer
	return (0);
}
