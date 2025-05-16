function observeElementInView(elementId, className, exeClassName) {
    const element = document.getElementById(elementId);
    const exeElement = document.getElementById(exeClassName);

    if (!element) return;

    const observer = new IntersectionObserver(
        (entries) => {
            entries.forEach((entry) => {
                if (entry.isIntersecting) {
                    exeElement.classList.add(className); // Add the class when in view
                } else {
                    exeElement.classList.remove(className); // Remove the class when out of view
                }
            });
        },
        { threshold: 0.5 } // Trigger when 50% of the element is visible
    );

    observer.observe(element);
}

observeElementInView('1topic', 'in-view', 'exe-1topic');
observeElementInView('2topic', 'in-view', 'exe-2topic');
observeElementInView('3topic', 'in-view', 'exe-3topic');
observeElementInView('4topic', 'in-view', 'exe-4topic');