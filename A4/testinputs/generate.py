import random

def generate_polynomial(degree):
    """Generates a random polynomial of the given degree."""
    terms = []
    for i in range(degree, -1, -1):
        coefficient = random.randint(-1000, 1000)
        if coefficient != 0:
            if i == 0:
                terms.append(f"{coefficient}")
            elif i == 1:
                terms.append(f"{coefficient}x")
            else:
                terms.append(f"{coefficient}x^{i}")

    return " + ".join(terms).replace("+ -", "- ")

def write_polynomials_to_files(num_files=100, max_degree=10):
    """Writes random polynomials to individual files."""
    for i in range(1, num_files + 1):
        degree = random.randint(2, max_degree)  # Random degree between 2 and max_degree
        polynomial = generate_polynomial(degree)
        with open(f"test{i}.txt", "w") as f:
            f.write(polynomial)

if __name__ == "__main__":
    write_polynomials_to_files()
